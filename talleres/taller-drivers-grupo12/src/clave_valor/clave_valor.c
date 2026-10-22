#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

#include <linux/mutex.h>

#define BUFFER_SIZE 32
#define CANTIDAD_CLAVES 16

// Estado global del modulo: una tabla de CANTIDAD_CLAVES casilleros
// compartida por todos los procesos que abran /dev/clave_valor (no una
// tabla por cada apertura).
static long tabla[CANTIDAD_CLAVES];
static struct mutex tabla_mutex;

// El read no recibe ninguna clave: cada proceso solo puede leer SU PROPIO
// casillero. Pista: calcular la clave usando el PID del proceso que llama.
static ssize_t clave_valor_read(struct file *filp, char __user *data, size_t size, loff_t *offset) {
  pid_t pid = task_tgid_nr(current); 
  int casillero = pid % CANTIDAD_CLAVES;
  long valor;
  mutex_lock(&tabla_mutex);
  valor = tabla[casillero];
  mutex_unlock(&tabla_mutex);

  char buf[32];
  int len = snprintf(buf, sizeof(buf), "%ld\n", valor); // Long a str
  copy_to_user(data, &buf, len);
  // Medio que ignoramos size 
  return len;
}

// El write si recibe una clave (como texto, en el buffer): la clave de la
// tabla que hay que incrementar en 1. No hace falta mandar nada mas: el
// incremento siempre es de a uno.
static ssize_t clave_valor_write(struct file *filp, const char __user *data, size_t size, loff_t *offset) {
  char clave_str[BUFFER_SIZE];
  if (0 != copy_from_user(&clave_str, data, size))
    return -EINVAL;
  int clave;
  if (0 != kstrtoint(clave_str, 0, &clave)) // str to int
    return -EINVAL;
  if (clave >= CANTIDAD_CLAVES || clave < 0)
    return -EINVAL;

  // Seccion critica
  {
    mutex_lock(&tabla_mutex);
    tabla[clave]++;
    mutex_unlock(&tabla_mutex);
  }

  return size;
}

static struct file_operations clave_valor_operaciones = {
  .owner = THIS_MODULE,
  .write = clave_valor_write,
  .read = clave_valor_read
};

static struct cdev clave_valor_device;
static dev_t major;
static struct class *clave_valor_class;

static int __init clave_valor_init(void) {
  mutex_init(&tabla_mutex);

  cdev_init(&clave_valor_device, &clave_valor_operaciones);
  alloc_chrdev_region(&major, 0, 1, "clave_valor");
  cdev_add(&clave_valor_device, major, 1);

  clave_valor_class = class_create(THIS_MODULE, "clave_valor");
  device_create(clave_valor_class, NULL, major, NULL, "clave_valor");


  return 0;
}

static void __exit clave_valor_exit(void) {
  mutex_destroy(&tabla_mutex);
  cdev_del(&clave_valor_device);
  unregister_chrdev_region(major, 1);

  device_destroy(clave_valor_class, major);
  class_destroy(clave_valor_class);
}

module_init(clave_valor_init);
module_exit(clave_valor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una base de datos clave-valor muy simplificada");

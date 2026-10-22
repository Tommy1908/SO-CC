#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

static ssize_t nulo_read(struct file *filp, char __user *data, size_t s, loff_t *off) {
  return 0;
}

static ssize_t nulo_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
  return s; // Devuelve cantidad de bytes como si hubiera escrito
}

static struct file_operations nulo_operaciones = {
  .owner = THIS_MODULE,
  // Completar
  .read = nulo_read,
  .write =  nulo_write
};

static struct cdev nulo_device;
static dev_t major; // En realidad contiene el major y el minor
static struct class *nulo_class;

static int __init hello_init(void) {
  
  // Inicializar como dispositivo
  //void cdev_init(struct cdev *dev, const struct file_operations *fops);
  cdev_init(&nulo_device, &nulo_operaciones); // char device, operaciones
	
  // Conseguir major y minor
  /*
  "El parámetro num corresponde al major, que nos será devuelto tras ejecutar la función. 
  Los parámetros firstminor y count podemos ponerlos respectivamente en 0 y 1, con lo cual el minor será 0.
  name, por su parte, corresponde a un nombre para el dispositivo."
  */
  alloc_chrdev_region(&major, 0, 1, "nulo" );

  // Asignamos
  // count: cantidad de números de dispositivo consecutivos que asociás a ese cdev.
  cdev_add (&nulo_device , major , 1 ); //no dice que es count

  // Ya inicializado
  // Crear los nodos correspondientes en el sistema de archivos
  nulo_class = class_create(THIS_MODULE, "nulo");
  device_create(nulo_class, NULL, major, NULL, "nulo");

  return 0;
}

static void __exit hello_exit(void) {
  device_destroy(nulo_class, major);
  class_destroy(nulo_class);
  cdev_del (&nulo_device);
  unregister_chrdev_region (major, 1 );
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de '/dev/null'");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <linux/mutex.h>

// Usamos el registro de datos del puerto paralelo.
#define LED_PORT 0x378
#define LED_BIT_ON 0x01

// Formato binario compartido con Python ("<BBBBf"), 8 bytes:
// status (0/1), rojo, verde, azul.
// En C de usuario: bool status; uint8_t r, g, b; float brillo;
struct led_estado {
  u8 status;
  struct {
    u8 r, g, b;
  } color;
  __le32 brillo;
};

// Estado global del modulo, compartido por todos los procesos que abran
// /dev/led. El campo status de aca no es la fuente de verdad: el
// encendido/apagado real vive en el bit LED_BIT_ON del puerto.
static struct led_estado estado = {
  .color = { .r = 0, .g = 255, .b = 0 },
  .brillo = cpu_to_le32(0x3f800000), // 1.0f
};

static struct cdev led_device;
static dev_t led_dev;
static struct class *led_class;

static struct mutex estado_mutex;

static ssize_t led_read(struct file *filp, char __user *data, size_t size,
                        loff_t *offset) {
  /* Completar:
   *  - Si ya se leyeron los 8 bytes (*offset >= sizeof(struct led_estado))
   *    o si size es 0, devolver 0 (fin de archivo).
   *  - Armar una copia del estado actual, con status tomado del bit
   *    LED_BIT_ON de LED_PORT (inb).
   *  - Copiar al usuario a partir de *offset, como mucho size bytes, 
        actualizar *offset y devolver cuantos se copiaron.
   */
  if (*offset >= sizeof(struct led_estado))
    return 0;
  if (size == 0)
    return 0;
  if (size > sizeof(struct led_estado))
    size = sizeof(struct led_estado);
    // Seccion critica
    struct led_estado copia_estado;
  {
    mutex_lock(&estado_mutex);
    copia_estado = estado;
    copia_estado.status = inb(LED_PORT);
    mutex_unlock(&estado_mutex);
  }

  int no_copiados = copy_to_user(data, (char *)&copia_estado + *offset, size); //char para sumar correctamente de 1 byte * offset
  int copiados = size - no_copiados;
  *offset += copiados;
  return copiados;
}

// Solo para el log: convierte un brillo ya validado a milesimas redondeadas.
static u32 brillo_milesimas(u32 bits) {
  u32 exponente = (bits >> 23) & 0xff;
  u32 desplazamiento;
  u64 escalado;

  // Estos valores son menores a media milesima (incluye ambos ceros).
  if (exponente < 116)
    return 0;

  desplazamiento = 150 - exponente;
  escalado = (u64) ((bits & 0x7fffff) | 0x800000) * 1000;
  return (escalado + (1ULL << (desplazamiento - 1))) >> desplazamiento;
}

static ssize_t led_write(struct file *filp, const char __user *data, size_t size,
                         loff_t *offset) {
  /* Completar:
   *  - Una escritura vacia devuelve 0; cualquier otro tamaño distinto de
   *    sizeof(struct led_estado) es -EINVAL.
   *  - Copiar la estructura desde el usuario.
   *  - Validar status (0 o 1) y brillo (entre 0.0 y 1.0, sin negativos,
   *    infinitos ni NaN) sin usar floats. Pista: para floats positivos,
   *    comparar los bits como enteros da el mismo orden que comparar los
   *    numeros. Ojo con -0.0 (0x80000000), que tambien es valido.
   *  - Guardar el nuevo estado y escribir el bit de encendido en LED_PORT
   *    (outb).
   *  - Registrar la escritura con printk, con el formato del enunciado
   *    (usar brillo_milesimas para el brillo).
   */
  if(size == 0){
    return 0;
  }
  else if(size != sizeof(struct led_estado)){
    return -EINVAL;
  }

  struct led_estado nuevo_estado;

  if (copy_from_user(&nuevo_estado, data, size))
    return -EFAULT;

  if (nuevo_estado.status > 1) {
    return -EINVAL;
  }

  u32 bits = le32_to_cpu(nuevo_estado.brillo);
  if (bits != 0x80000000 && bits > 0x3F800000) // 0x3F800000 = 1.0 // 0x80000000 = -0
    return -EINVAL;

  // Seccion critica
  {
    mutex_lock(&estado_mutex);
    estado = nuevo_estado;
    outb(nuevo_estado.status, LED_PORT);
    printk("led: LED %s, RGB(%d, %d, %d), brillo=0.%d", nuevo_estado.status ? "ON" : "OFF", nuevo_estado.color.r, nuevo_estado.color.g, nuevo_estado.color.b, brillo_milesimas(bits));
    mutex_unlock(&estado_mutex);
  }

  return size;
}

static struct file_operations led_operaciones = {
  .owner = THIS_MODULE,
  .read = led_read,
  .write = led_write
};

static int __init led_init(void) {

  /* Completar: crear el character device y dejar el LED apagado. */

  cdev_init(&led_device, &led_operaciones); // char device, operaciones
  alloc_chrdev_region(&led_dev, 0, 1, "led" );
  cdev_add (&led_device , led_dev , 1 ); //no dice que es count

  led_class = class_create(THIS_MODULE, "led");
  device_create(led_class, NULL, led_dev, NULL, "led");

  // Apagar led
  outb(0, LED_PORT);

  mutex_init(&estado_mutex);

  return 0;
}

static void __exit led_exit(void) {
  /* Completar: apagar el LED y destruir el character device. */
  // Apagar led
  outb(0, LED_PORT);
  
  device_destroy(led_class, led_dev);
  class_destroy(led_class);
  cdev_del (&led_device);
  unregister_chrdev_region (led_dev, 1 );

  mutex_destroy(&estado_mutex);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("LED controlado por el puerto paralelo con outb e inb");

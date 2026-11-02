Indicar las acciones que debe tomar el administrador de E/S:
a) cuando se efectúa un open().
b) cuando se efectúa un write().

a) open()
1. El proceso invoca la syscall open().
2. El administrador de E/S identifica el dispositivo solicitado.
3. Verifica que el dispositivo exista y que el proceso tenga permisos para acceder.
4. Localiza el driver correspondiente.
5. Invoca driver_open().
6. Si tiene éxito, registra la apertura y devuelve un descriptor al proceso (en una interfaz basada en descriptores).
driver_init() no se ejecuta con cada open(). Se ejecuta cuando se carga el driver, segun la API de la practica.
b) Cuando se ejecuta write()
1. El proceso invoca write().
2. El administrador de E/S identifica el dispositivo asociado.
3. Verifica que la operacion este permitida.
4. Invoca driver_write() del driver correspondiente.
5. El driver interactua con el controlador del dispositivo mediante IN() y OUT(), segun corresponda.
6. El administrador devuelve el resultado de la operación al proceso.

Diferencia:
- Administrador de E/S: coordina las solicitudes y selecciona el driver.
- Driver: conoce como comunicarse con el hardware especifico.
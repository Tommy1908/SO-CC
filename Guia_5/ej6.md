¿Cuál debería ser el nivel de acceso para las syscalls IN y OUT? ¿Por qué?

Kernel pues IN() y OUT() requieren privilegios, porque permiten acceder directamente al hardware. Los procesos de usuario deben hacerlo indirectamente a través de las syscalls de E/S, que permiten al SO controlar y proteger el acceso a los dispositivos. Sino un usuario podria romper todo.

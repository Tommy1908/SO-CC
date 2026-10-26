Ejercicio 2
Los sistemas de autenticación remota, donde el usuario se debe autenticar a través de un canal inseguro (que puede ser visto por terceros), como por ejemplo POP3, a menudo no transmiten la contraseña en el proceso de autenticación para evitar que ésta sea interceptada.

a) ¿Sería seguro autenticar al usuario enviando por el canal (desde el usuario hasta el sistema) el
hash de la contraseña? ¿A qué ataque es vulnerable este esquema?
Si envia el hash en el medio es vulnerable a Man in the middle, copia el hash y lo envia el (como paquete imagino). Aclara que se llama Replay Attack
b) Un esquema Challenge-Response basado en una función de hash opera de la siguiente manera:
El sistema envía una cadena seed elegida al azar.
El usuario responde con el resultado de hash(seed + contraseña).
El sistema hace la misma cuenta de su lado y verifica el resultado.
Si un atacante captura toda esta conversación, ¿sería capaz de realizar un ataque de fuerza
bruta sobre la contraseña sin realizar ninguna interacción con el servidor (ataque offline)?
Si, porque sabe la seed y sabe el resultado
seed + pass = hash_objetenido
Tiene que ir probando hasta adivinar pass
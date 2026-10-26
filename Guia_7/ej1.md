En muchos sistemas las contraseñas de los usuarios no se almacenan, sino que se almacena solo el
resultado de una función de hash. Suponiendo que la función de hash utilizada entrega valores de 64
bits y que los resultados se encuentran bien distribuidos, responder

a) ¿Cómo puede verificar el sistema si un usuario ingresó su contraseña correctamente, si no la
tiene almacenada en el sistema?
Tiene que aplicar la misma funcion de hash y verificar que sean iguales.

b) Si bien existen varias contraseñas que entregan el mismo valor de hash (colisión), ¿qué probabilidad hay de acertar el valor de hash almacenado?
Si estan bien distribuidos 1/2^64

c) ¿En cuántos años se puede tener un 50 % de probabilidad de haber acertado al valor de hash
almacenado, dado que pueden probarse mil millones de contraseñas por segundo?
Hay 2^64 hashes.
Para tener el 50% de probabilidad, tendrias que ya haber probado la mitad. 2^64/2 = 2^63.
2^63/10^9 (mil millones) -> 292 años

d) Conocer el valor de hash de una contraseña no implica conocer una contraseña que genere ese
valor. Suponer que se pueden probar contraseñas a la misma velocidad que en el ítem anterior,
pero nos informan que la contraseña es de, a lo sumo, 6 caracteres, y que estos solo son letras
minúsculas o dígitos. ¿Cuánto tiempo hay que esperar para averiguar la clave?
Si podes probar mil millones por segundo, (y no te bloquearan lo cual nunca entiendo como anda) tenes que hay 26 letras minusculas (sin ñ), y 10 digitos. Por cada slot hay 36 posibles combinaciones con repetidos. 36^6 es si es 6 digitos.
Hay 36+36²+36³+..+36⁶ contraseñas posibles. 2.238.976.116.
2.23 segs medio la cuenta

# Describir workflow

# Describir proceso de PR



# Nomenclatura de commits
### LOS COMMITS DEBERÁN ESTAR REDACTADOS ÍNTEGRAMENTE EN INGLÉS

Agregar una palabra clave descriptiva a cada commit:

* `feat` para cuando haya un cambio. 
* `fix` cuando se arregle algún bug o algún error de un commit previo.
* `docs` cuando se hagan cambios de documentación, ya sea de _docstring_ o de _md_.
* `test` para cuando se hagan agreguen o se hagan modificaciones en los test del código
* Agregar a la etiqueta un `!` cuando el commit realiza un cambio importante en el código que puede llevar a que el proyecto deje de funcionar (cambio de API key, cambios de configuraciones, modificación de métodos de alto nivel). Sirve para que al revisarse en el PR no se lo pase por arriba y se concentre en el commit que puede provocar un cambio radical en el proyecto.


*Ejemplo:*
```
git commit -m "feat: create a new service"
```

# Configuraciones de entorno
Agregar aquí, todas las configuraciones públicas del proyecto. Por ejemplo, si se utiliza un `.env`, crear una entrada que lo describa. Si se utiliza una API key, indicar cómo se configura.

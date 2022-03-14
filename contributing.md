# Seguí este workflow para crear ramas y realizar pruebas locales
https://docs.google.com/presentation/d/1uC4vF8jFkpqOZ23hXQFJVnZPfnTCZgvqXFlYv8x6j2Y/edit#slide=id.g118b171e085_0_34
https://www.notion.so/Git-y-GitHub-57819f890e924ad2bc7217f772cab900#8315411b2ae343d6a08e540c3d40a85d

# Creación de un Pull Request
* Antes de realizar un Pull Request, por favor solucioná todos los conflictos y probá manualmente todas las funcionalidades de la nueva branch.
* Utilizá este [template](https://github.com/maxiyommi/labo_inteligente/blob/patch/create-pull-request-template/docs/pull_request_template.md) para crear tu PR. En breve lo vamos a configurar para que el template se muestre automáticamente.
* Considerá como una posibilidad, [realizar un rebase interactivo y squash](https://dev.to/amalv/a-cleaner-github-workflow-one-commit-per-pull-pequest-3ic1), para mantener el historial de commits lo más acotado posible


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

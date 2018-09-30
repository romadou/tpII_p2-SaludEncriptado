# Sistema de sensado de variables de salud por encriptación

Trabajo en progreso

Para correr:
  * Tener instalado `ruby`
  * Tener instalad `rails`
  * En el directorio raíz del proyecto correr `bundle install` para instalar las dependencias
  * Luego correr `rails db:create db:migrate` para crear la base de datos y las tablas
  * Luego `rails s`
    * Dado que aun esta en progreso, para crear algunos datos hacerlo via consola `rails c`

## API

Los endpoints de la api son los siguientes:

Metodo|Ruta|Descripción
------|----|-----------
GET   |/v1/patients|Lista todos los pacientes del sistema
POST  |/v1/patients|Crea un paciente
GET   |/v1/patients/:id|Muestra un paciente con todos sus datos y asociaciones
PATCH |/v1/patients/:id|Actualiza el paciente pasado en la ruta
PUT   |/v1/patients/:id|Actualiza el paciente pasado en la ruta
DELETE|/v1/patients/:id|Eliminar el paciente pasado en la ruta
GET   |/v1/sensors|Lista todos los sensores
POST  |/v1/sensors|Crea un sensor
GET   |/v1/sensors/:id|Muestra el sensor pasado en la ruta
PATCH |/v1/sensors/:id|Actualiza el sensor pasado en la ruta
PUT   |/v1/sensors/:id(.:format)       api/v1/sensors#update
DELETE|/v1/sensors/:id(.:format)       api/v1/sensors#destroy
GET   |/v1/measures(.:format)          api/v1/measures#index
POST  |/v1/measures(.:format)          api/v1/measures#create
GET   |/v1/measures/:id(.:format)      api/v1/measures#show
DELETE|/v1/measures/:id(.:format)      api/v1/measures#destroy
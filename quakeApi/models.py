from django.db import models

    
class Sismo(models.Model):
    fecha_local = models.DateTimeField()  # Nueva línea para almacenar la fecha local
    fecha_utc = models.DateTimeField()    # Cambiando el nombre de 'fecha' a 'fecha_utc' para claridad
    ubicacion = models.CharField(max_length=255)
    latitud = models.FloatField()
    longitud = models.FloatField()
    profundidad = models.FloatField()
    magnitud = models.FloatField()

    def __str__(self):
        return f"{self.fecha_utc} - Magnitud: {self.magnitud}"
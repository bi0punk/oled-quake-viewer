from django.db import models

class Quake(models.Model):
    fecha_local = models.DateTimeField()
    fecha_utc = models.DateTimeField()
    lugar = models.CharField(max_length=255)
    latitud = models.DecimalField(max_digits=9, decimal_places=6)
    longitud = models.DecimalField(max_digits=9, decimal_places=6)
    profundidad = models.DecimalField(max_digits=8, decimal_places=2)
    magnitud = models.DecimalField(max_digits=4, decimal_places=2)

    def __str__(self):
        return f"{self.lugar} - {self.fecha_local.strftime('%Y-%m-%d %H:%M:%S')}"

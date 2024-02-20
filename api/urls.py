from django.urls import path
from .views import UltimoDato
from .views import RecolectarDatosSismologicos
urlpatterns = [
    path('ultimo_dato/', UltimoDato.as_view(), name='ultimo_dato'),
    path('recolectar-datos/', RecolectarDatosSismologicos.as_view(), name='recolectar-datos-sismologicos'),
]

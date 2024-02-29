from django.urls import path
from .views import ListaSismos, DetalleSismo  # Importación correcta desde views.py
from . import views
urlpatterns = [
    path('sismos/', ListaSismos.as_view(), name='lista-sismos'),
    path('sismos/<int:pk>/', DetalleSismo.as_view(), name='detalle-sismo'),
    path('', views.mi_nueva_vista, name='api'), 
]
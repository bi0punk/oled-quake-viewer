from rest_framework import serializers
from .models import Libro
from .models import Sismo

class LibroSerializer(serializers.ModelSerializer):
    class Meta:
        model = Libro
        fields = '__all__'


class SismoSerializer(serializers.ModelSerializer):
    class Meta:
        model = Sismo
        fields = '__all__'
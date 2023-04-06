from django.forms import ImageField
from rest_framework import serializers
from datetime import datetime
from app.models import Image, Type

# Serializers define the API representation.
class UploadSerializer(serializers.Serializer):
    file_uploaded = ImageField()
    class Meta:
        fields = ['file_uploaded']
        
class TypeSerializer(serializers.ModelSerializer):
    class Meta:
        model = Type
        fields = '__all__'

class ImageSerializer(serializers.ModelSerializer):
    type = TypeSerializer()
    image_url = serializers.ReadOnlyField()
    # formatted_date = serializers.SerializerMethodField()
    
    class Meta:
        model = Image
        fields = ['id', 'name', 'image','image_url', 'date','type']

    # def get_formatted_date(self, obj):
    #     return datetime.strftime(obj.date, '%Y-%m-%d %H:%M:%S')
    def to_representation(self, instance):
        representation = super().to_representation(instance)
        representation.pop("image")

        return representation
    


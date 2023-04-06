from django.db import models
from datetime import datetime
from cloudinary.models import CloudinaryField

class Type(models.Model):
    name = models.CharField(max_length=50)
    
class Image(models.Model):
    name = models.CharField(max_length=255)
    image = CloudinaryField("image")
    date = models.DateField(auto_now_add=True, null=True)
    type = models.ForeignKey(Type, on_delete=models.CASCADE, null=True)
    @property
    def image_url(self):
        return (
            f"https://res.cloudinary.com/dm7tnmhj4/image/upload/{self.image}.jpg"
        )
    
    
    
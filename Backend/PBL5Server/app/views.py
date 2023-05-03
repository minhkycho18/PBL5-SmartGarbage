from rest_framework.viewsets import ViewSet, ModelViewSet
from rest_framework.response import Response
from django.conf import settings
from .serializers import UploadSerializer, ImageSerializer
from channels.generic.websocket import WebsocketConsumer
from channels.layers import get_channel_layer
from asgiref.sync import async_to_sync

from app.paginations import MyPagination
from app.models import Image, Type
import cloudinary
cloudinary.config(
  cloud_name = "dm7tnmhj4",
  api_key = "715756646867576",
  api_secret = "TjUu7LzW21cddLWQ45FHxm3eH3k",
  secure = True
)
import cloudinary.api
import cloudinary.uploader

from datetime import datetime
from keras.models import load_model
from keras.applications.mobilenet_v2 import preprocess_input
import cv2
import numpy as np
import os

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


# ViewSets define the view behavior.
class UploadViewSet(ViewSet):
    serializer_class = UploadSerializer

    def create(self, request):
        file_uploaded = request.FILES.get('file_uploaded')
        model_path = BASE_DIR + "\\app\\garbage_train.model"
        print(BASE_DIR + "\\app\\garbage_train.model")

        # image_path =BASE_DIR + image.image.url.replace('/', '\\')
        # print(BASE_DIR + image.image.url.replace('/', '\\'))
        image_path = os.path.join(settings.MEDIA_ROOT, "image.jpg")
        
                
        uploaded_image = cloudinary.uploader.upload(file_uploaded, folder='my_folder')
        print(image_path.replace('/', '\\'))

        with open(image_path, 'wb+') as destination:
            for chunk in file_uploaded.chunks():
                destination.write(chunk)
        model = load_model(model_path)
        image = cv2.imread(image_path)
        image = cv2.resize(image, (224, 224))
        image = np.expand_dims(image, axis=0)
        image = image.astype("float32")
        image = preprocess_input(image)

        result = model.predict(image)

        
        if result[0][0] > result[0][1]:
            print("tái chế")
            # imageType = Type.objects.get(id=1)
            # image = Image(name=file_uploaded.name, image=file_uploaded, type=imageType)
            # image.save()
            return Response("tái chế")
        else:
            print("không tái chế")
            # imageType = Type.objects.get(id=2)
            # image = Image(name=file_uploaded.name, image=uploaded_image['public_id'], type=imageType)
            # image.save()
            return Response("Không tái chế")
            

class NotifyViewSet(ViewSet):
    
    def list(self, request):
        message = 'Full'
        # Broadcast message to all clients in the room
        channel_layer = get_channel_layer()
        async_to_sync(channel_layer.group_send)(
            'test',
            {
                'type': 'chat_message',
                'message': message
            }
        )
        return Response('OK')
    

class ImageViewSet(ModelViewSet):
    queryset = Image.objects.all()
    serializer_class = ImageSerializer
    pagination_class = MyPagination

    def list(self, request):
        date_req = self.request.query_params.get('date', None)
        if date_req:
            dateSearch = datetime.strptime(date_req, '%Y-%m-%d').date()
            image = self.queryset.filter(date__exact=dateSearch)
        else:
            image = self.queryset.all()
        paginator = self.pagination_class()
        page = paginator.paginate_queryset(image, request)

        serializer = self.serializer_class(page, many=True)
        
        
        return paginator.get_paginated_response(serializer.data)

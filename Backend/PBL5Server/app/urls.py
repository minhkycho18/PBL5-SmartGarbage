from django.urls import path, include
from rest_framework import routers
from .views import UploadViewSet,ImageViewSet


router = routers.DefaultRouter()
router.register(r'upload', UploadViewSet, basename="upload")
router.register(r'images', ImageViewSet, basename='image')

# Wire up our API using automatic URL routing.
urlpatterns = [
    path('', include(router.urls)),
]
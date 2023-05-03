from django.urls import path, include
from rest_framework import routers
from .views import UploadViewSet,ImageViewSet, NotifyViewSet


router = routers.DefaultRouter()
router.register(r'upload', UploadViewSet, basename="upload")
router.register(r'images', ImageViewSet, basename='image')
router.register(r'notify', NotifyViewSet, basename='notify')

# Wire up our API using automatic URL routing.
urlpatterns = [
    path('', include(router.urls)),
]
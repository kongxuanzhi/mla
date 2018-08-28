常见问题：
1、打不开视频 ---	
  CvCapture *capture = cvCreateFileCapture("开不了口.mp4"); 失败
  解决方法，将 opencv/bin/opencv_ffmpeg310.dll 拷到c:\\windows\system32\  下面就好了
  原因：opencv更擅长图像处理和识别，视频编解码还是要靠ffmpeg来做到
  
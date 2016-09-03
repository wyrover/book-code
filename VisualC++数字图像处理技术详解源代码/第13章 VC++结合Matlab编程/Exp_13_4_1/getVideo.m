function pixel = getVideo
% 读取视频文件
avi = aviread('samplevideo.avi');
% 获得视频数据并转成cell类型
video = {avi.cdata};
% 将像素数据转化为实数数组
pixels = double(cat(4,video{1:2:end}))/255;
% 清除视频数据
clear video

% 获取视频图像的行数
rows = size(pixels,1);
% 获取视频图像的列数
cols = size(pixels,2);
% 获取视频的帧数
frames = size(pixels,4);

% 将视频中每一帧图像由彩色转为灰度
pixel = zeros(rows,cols,frames);
for f = 1:frames
    pixel(:,:,f) = (rgb2gray(pixels(:,:,:,f)));
end
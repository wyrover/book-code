function tracking(pixel)
% 获取视频图像的行数
rows = size(pixel,1);
% 获取视频图像的列数
cols = size(pixel,2);
% 获取视频的帧数
frames = size(pixel,3);

% 循环处理视频中的每一帧，并用红色方框框选出运动物体
d = zeros(rows,cols,frames);
bw = zeros(rows,cols,frames);
for i = 2:frames
    % 求出相邻两帧之间的差
    d(:,:,i)=(abs(pixel(:,:,i)-pixel(:,:,i-1)));
    % 将两帧差图转化为二值图像
    k = d(:,:,i);
    bw(:,:,i) = im2bw(k, .2);
    % 标注二进制图像中已连接的部分
    bwlabel(bw(:,:,i));
    % 去除图像中的噪声点
    bw(:,:,i) = medfilt2(bw(:,:,i));
    % 显示图像
    imshow(bw(:,:,i));
    
    % 开启加图功能
    hold on
    
    frtFound = 1;
    height = 0;
    top = 0;
    for h = 1:rows
        for w = 1:cols
            if(bw(h,w,i)>0.5)
                % 记录运动物体最下点位置
                height = h;
                if(frtFound == 1)
                    % 记录运动物体最上点位置
                    top = h;
                    frtFound = 0;
                end
                break;
            end
        end
    end

    frtFound = 1;
    width = 0;
    left = 0;
    for w = 1:cols
        for h = 1:rows
            if(bw(h,w,i)>0.5)
                % 记录运动物体最右点位置
                width = w;
                if(frtFound == 1)
                    % 记录运动物体最左点位置
                    left = w;
                    frtFound = 0;
                end
                break;
            end
        end
    end

    % 计算运动物体的高度
    realHeight = height - top;
    % 计算运动物体的宽度
    realWidth = width - left;    

    % 运动物体中心点纵坐标
    heightCenter = top + realHeight / 2;
    % 运动物体中心点横坐标
    widthCenter = left + realWidth / 2;
    
    % 用红色矩形框框住运动物体
    rectangle('Position',[left top realWidth realHeight],'EdgeColor','r');
    % 在红色矩形框中心画红色*号标志
    plot(widthCenter, heightCenter, 'r*');
    
    % 强制绘图
    drawnow;
    
    % 关闭加图功能
    hold off
end




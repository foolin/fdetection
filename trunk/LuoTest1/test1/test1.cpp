// test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "math.h"
//#include "iostream.h"
 
//所有的以新风格命名的函数都在 cv 命名空间中
//如果希望不要每次都输入 cv:: ，则可使用下面语句
using namespace cv;

struct DataField
{
	int x;		//记录图像的某个像素的横坐标
	int y;		//记录图像的某个像素的纵坐标
	double value;	//记录图像某个像素转化为数据场后的势值
};

void BubbleSort(DataField *a,int n)
{
	int i,j,flag=1;
	DataField temp;

	for(i=1;i<n&&flag==1;i++)
	{
		flag=0;
		for(j=0;j<n-i;j++)
		{
			if(a[j].value<a[j+1].value)
			{
				flag=1;
				temp=a[j];						
				a[j]=a[j+1];		
				a[j+1]=temp;
				
			}
		}
	}
}

DataField Data(char* name)
{ 
 
    const char* imagename = name;
 
	IplImage * img =cvLoadImage(imagename,0);		//以单通道的格式读取图像
    IplImage * img2=0;
	
	int height,width,step,channels; //记录图像长、宽、行长度、信道信息的变量
	uchar *data1,*data2;	//记录图像数据的数组	 
	int i,j,l,m,p=0,q=0,x,y;	//用于遍历图像时，for循环的变量
	double sum=0,temp;		//建立数据场，运算势值的临时变量
	DataField result;		//用以保存一幅图像运算后，若干个局部极值点投影成一个点
	result.value=0;
	result.x=0;
	result.y=0;
	int sum_x=0,sum_y=0;
	
    if(!img)
    {
        fprintf(stderr, "Can not load image %s\n", imagename);
        return	result;
    }
 
	//获取图像数据
	height=img->height;
	width=img->width;
	step=img->widthStep;
	channels=img->nChannels;
	data1=(uchar * )img->imageData;

	//用DataField类型的数组，存放图像中各个像素点，计算后的势值及其坐标
	DataField * gettemp=new DataField[height*width]; 
	DataField * local_max_point=new DataField[(height)*(width)];

	for(i=0;i<(height)*(width);i++)
	{
		gettemp[i].value=0;	
	}

	//复制图像
	img2=cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	data2=(uchar *)img2->imageData;
	
/*   //二值化图像
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)	
			if(data1[i*step+j]>120)
				data1[i*step+j]=255;
			else
				data1[i*step+j]=0;
*/

   //遍历图像，创建数据场,sigema=1
	for(y=3;y<height-3;y++)
	{
		for(x=3;x<width-3;x++)
		{
			for(l=y-3;l<y+3;l++)
			{
				for(m=x-3;m<x+3;m++)
				{				
					temp=-(((l-y)*(l-y)+(m-x)*(m-x))/2);		//e的指数					
					data2[y*step+x]+=(uchar)(255-(data1[l*step+m]))*exp(temp);
					//把计算的势值存放在gettemp数组中的value域中,
					/* 
					test[q]+=(255-(data1[l*step+m]))*exp(temp);
					check[q].value=test[q];
					*/
					gettemp[q].value+=(255-(data1[l*step+m]))*exp(temp);
					gettemp[q].x=x;
					gettemp[q].y=y;					
				}	
			}
		q++;
		}
	}

	//假定势值大于800的点，为局部极大值
	for(i=0;i<(height-6)*(width-6);i++)
		if(gettemp[i].value>800)
		{
			local_max_point[p].value=gettemp[i].value;
			local_max_point[p].x=gettemp[i].x;
			local_max_point[p].y=gettemp[i].y;
			p++;
		}

/*	BubbleSort(gettemp,(height-6)*(width-6));
	double Max=gettemp[0].value*0.7;	//假定势值中最大值Max，而0.8Max为临界值，将势值大于0.8Max的点视为局部极大值
	i=0;
	
	while(gettemp[i].value>Max)
	{
		local_max_point[p].value=gettemp[i].value;
		local_max_point[p].x=gettemp[i].x;
		local_max_point[p].y=gettemp[i].y;
		p++;
		i++;
	}
*/
	printf("\n%d\n%d\n",i,p);

	for(i=0;i<p;i++)
	{
		sum_x+=local_max_point[i].x*local_max_point[i].value;
		sum_y+=local_max_point[i].y*local_max_point[i].value;
		sum+=local_max_point[i].value;
	}
	result.value=sum/p;
	result.x=sum_x/sum;
	result.y=sum_y/sum;

	printf("%f\t%d%d",result.value,result.x,result.y);
	printf("\n");	

	//cvSaveImage("J_result/g.jpg",img2);
	
	namedWindow("image", CV_WINDOW_AUTOSIZE); //创建窗口
	//namedWindow("image2", CV_WINDOW_AUTOSIZE); //创建窗口
	imshow("image", img); //显示图像
	imshow("image2", img2); //显示图像
 
    
	delete gettemp;
	delete local_max_point;
//	cvDestroyWindow( "image" );//销毁窗口
    cvReleaseImage( &img ); //释放图像
	//cvDestroyWindow( "image2" );//销毁窗口
    cvReleaseImage( &img2 ); //释放图像

	return result;
}

void main()
{
	DataField temp[10];
	temp[0]=Data("Faces/a.jpg");
	temp[1]=Data("Faces/b.jpg");
	temp[2]=Data("Faces/c.jpg");
	temp[3]=Data("Faces/d.jpg");
	temp[4]=Data("Faces/e.jpg");
	temp[5]=Data("Faces/f.jpg");
	temp[6]=Data("Faces/g.jpg");
	temp[7]=Data("Faces/h.jpg");
	temp[8]=Data("Faces/i.jpg");
	temp[9]=Data("Faces/j.jpg");
	
	double second_datafield [100] [100];
	for(int i=0;i<100;i++)
		for(int j=0;j<100;j++)
			second_datafield[i][j]=0;

	for(int y=0;y<100;y++)
		for(int x=0;x<100;x++)
			for(int k=0;k<6;k++)
			{
				double d=(temp[k].x-x)*(temp[k].x-x)+(temp[k].y-y)*(temp[k].y-y);	//距离的平方				
				if(d<=81)	//假设sigema=3
				{
					second_datafield[x][y]+=temp[k].value*exp(-d/18);
				}
			}

/*	for(int y=0;y<100;y++)
		for(int x=0;x<100;x++)
		{
			if(second_datafield[x][y]!=0)
			printf("%f%s%d%s%d%s\t",second_datafield[x][y],"(",x,",",y,")");
		}
		*/
		printf("\n\n\n");
		for(int i=0;i<10;i++)
			printf("%f\t%s%d%s%d%s\t%c\n",second_datafield[temp[i].x][temp[i].y],"(",temp[i].x,",",temp[i].y,")",'a'+i);
			
	//printf("%f\t%d %d",temp.value,temp.x,temp.y);
	waitKey();
	//return 0;
}
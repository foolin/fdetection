// test1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "math.h"
//#include "iostream.h"
 
//���е����·�������ĺ������� cv �����ռ���
//���ϣ����Ҫÿ�ζ����� cv:: �����ʹ���������
using namespace cv;

struct DataField
{
	int x;		//��¼ͼ���ĳ�����صĺ�����
	int y;		//��¼ͼ���ĳ�����ص�������
	double value;	//��¼ͼ��ĳ������ת��Ϊ���ݳ������ֵ
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
 
	IplImage * img =cvLoadImage(imagename,0);		//�Ե�ͨ���ĸ�ʽ��ȡͼ��
    IplImage * img2=0;
	
	int height,width,step,channels; //��¼ͼ�񳤡����г��ȡ��ŵ���Ϣ�ı���
	uchar *data1,*data2;	//��¼ͼ�����ݵ�����	 
	int i,j,l,m,p=0,q=0,x,y;	//���ڱ���ͼ��ʱ��forѭ���ı���
	double sum=0,temp;		//�������ݳ���������ֵ����ʱ����
	DataField result;		//���Ա���һ��ͼ����������ɸ��ֲ���ֵ��ͶӰ��һ����
	result.value=0;
	result.x=0;
	result.y=0;
	int sum_x=0,sum_y=0;
	
    if(!img)
    {
        fprintf(stderr, "Can not load image %s\n", imagename);
        return	result;
    }
 
	//��ȡͼ������
	height=img->height;
	width=img->width;
	step=img->widthStep;
	channels=img->nChannels;
	data1=(uchar * )img->imageData;

	//��DataField���͵����飬���ͼ���и������ص㣬��������ֵ��������
	DataField * gettemp=new DataField[height*width]; 
	DataField * local_max_point=new DataField[(height)*(width)];

	for(i=0;i<(height)*(width);i++)
	{
		gettemp[i].value=0;	
	}

	//����ͼ��
	img2=cvCreateImage(cvGetSize(img),img->depth,img->nChannels);
	data2=(uchar *)img2->imageData;
	
/*   //��ֵ��ͼ��
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)	
			if(data1[i*step+j]>120)
				data1[i*step+j]=255;
			else
				data1[i*step+j]=0;
*/

   //����ͼ�񣬴������ݳ�,sigema=1
	for(y=3;y<height-3;y++)
	{
		for(x=3;x<width-3;x++)
		{
			for(l=y-3;l<y+3;l++)
			{
				for(m=x-3;m<x+3;m++)
				{				
					temp=-(((l-y)*(l-y)+(m-x)*(m-x))/2);		//e��ָ��					
					data2[y*step+x]+=(uchar)(255-(data1[l*step+m]))*exp(temp);
					//�Ѽ������ֵ�����gettemp�����е�value����,
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

	//�ٶ���ֵ����800�ĵ㣬Ϊ�ֲ�����ֵ
	for(i=0;i<(height-6)*(width-6);i++)
		if(gettemp[i].value>800)
		{
			local_max_point[p].value=gettemp[i].value;
			local_max_point[p].x=gettemp[i].x;
			local_max_point[p].y=gettemp[i].y;
			p++;
		}

/*	BubbleSort(gettemp,(height-6)*(width-6));
	double Max=gettemp[0].value*0.7;	//�ٶ���ֵ�����ֵMax����0.8MaxΪ�ٽ�ֵ������ֵ����0.8Max�ĵ���Ϊ�ֲ�����ֵ
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
	
	namedWindow("image", CV_WINDOW_AUTOSIZE); //��������
	//namedWindow("image2", CV_WINDOW_AUTOSIZE); //��������
	imshow("image", img); //��ʾͼ��
	imshow("image2", img2); //��ʾͼ��
 
    
	delete gettemp;
	delete local_max_point;
//	cvDestroyWindow( "image" );//���ٴ���
    cvReleaseImage( &img ); //�ͷ�ͼ��
	//cvDestroyWindow( "image2" );//���ٴ���
    cvReleaseImage( &img2 ); //�ͷ�ͼ��

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
				double d=(temp[k].x-x)*(temp[k].x-x)+(temp[k].y-y)*(temp[k].y-y);	//�����ƽ��				
				if(d<=81)	//����sigema=3
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
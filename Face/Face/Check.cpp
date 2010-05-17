#include "StdAfx.h"
#include "Check.h"
#include "String.h"

//���е����·�������ĺ������� cv �����ռ���
//���ϣ����Ҫÿ�ζ����� cv:: �����ʹ���������
//using namespace cv;

CCheck::CCheck(void)
{
}

CCheck::~CCheck(void)
{
}


//��ʼ��ͼ��
void CCheck::Init()
{
	m_ReadImage1 = NULL;
	m_ReadImage2 = NULL;
	m_ReadImage2 = NULL;

}


bool CCheck::Load(CString strPath)
{
	if(strPath.IsEmpty())
	{
		return false;
	}
	IplImage *image;
	image = cvLoadImage( strPath, 0);
	if(!image)
		return false;
//	SetImage(image);
	cvReleaseImage(&image);
	return true;
}
/*
bool CCheck::SetImage( IplImage* image, int maxWidth, int maxHeight )
{
	if(!image)
		return false;

	//����ReadImage
	if(!SetReadImage(image))
		return false;
	//����GrayImage
	if(!SetGrayImage())
		return false;
	//����BinaryImage
	if(!SetBinaryImage())
		return false;

	return true;
}
*/

//�����ð������
void CCheck::BubbleSort(DataField *a,int n)
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

DataField CCheck:: Data(CString name,int num) 
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
	result.number=num;
	int sum_x=0,sum_y=0;
	
	//�����ͼ������������ʾ��Ϣ���˳�����
    if(!img) 
    {
        fprintf(stderr, "Can not load image %s\n", imagename);
        return	result;
		
	}
 
	//��ȡͼ����Ϣ
	height=img->height;
	width=img->width;
	step=img->widthStep;
	channels=img->nChannels;
	data1=(uchar * )img->imageData;

	//��DataField���͵����飬���ͼ���и������ص㣬��������ֵ��������
	//gettemp���Դ�ţ�ͼ�񴴽�Ϊ���ݳ��󣬳��и����ֵ������
	DataField * gettemp=new DataField[height*width]; 
	//local_max_point���Դ�ţ����ݳ��еľֲ�����ֵ��������
	DataField * local_max_point=new DataField[(height)*(width)];
	
	//��gettemp��local_max_point���г�ʼ��
	for(i=0;i<(height)*(width);i++)
	{
		gettemp[i].number=num;
		gettemp[i].value=0;	
		local_max_point[i].value=0;
		local_max_point[i].x=0;
		local_max_point[i].y=0;
	}

    //ͼ�����ذ��й�һ��[0,1]
	double * picture_mat=new double [height*width];
	for(y=0;y<height;y++)
	{
		double element=0;	//ͼ������ÿ��ƽ�����
		for(x=0;x<width;x++)
		{
			element+=data1[y*step+x]*data1[y*step+x];
		}
		for(x=0;x<width;x++)
		{
			picture_mat[y*step+x]=data1[y*step+x]/sqrt(element);
		}
	}

	//����ͼ��
	img2=cvCreateImage(cvGetSize(img),img->depth,0);
	data2=(uchar *)img2->imageData;
	cvCopy(img,img2,0);

   //����ͼ�񣬴������ݳ�,sigema*sigema=10
	//��������forѭ�����Ա�������ͼƬ
	for(y=8;y<height-8;y++)		
	{
		for(x=8;x<width-8;x++)
		{
			//��(x,y)Ϊ���ĵ㣬��17*17�������У�������������ĵ㴦��������ֵ
			for(l=y-8;l<y+8;l++)
			{
				for(m=x-8;m<x+8;m++)
				{				
					//(l-y)*(l-y)+(m-x)*(m-x)Ϊ��ǰ�㵽���ĵ�ľ����ƽ��
					temp=-(((l-y)*(l-y)+(m-x)*(m-x))/10);		//e��ָ��					
					gettemp[y*step+x].value+=picture_mat[l*step+m]*exp(temp);	//���ݳ���(x,y)������ֵ
					gettemp[y*step+x].x=x;	//���ݳ��иõ�x����
					gettemp[y*step+x].y=y;	//���ݳ��иõ�y����				
				}	
			}
		}
	}
	
    //local_tempΪ���ֲ���ֵ�����ʱ����
	DataField * local_temp=new DataField[(height-40)*(width-40)];
	i=0;
	q=0;
	//���ݳ�����Ҫ���ֲ���ֵ������ĵ㣬���local_temp��
	for(y=20;y<height-20;y++)
		for(x=20;x<width-20;x++)
			local_temp[i++]=gettemp[y*step+x];
					
	while(q<20)
	{
		BubbleSort(local_temp,i);
		//ͨ�������ҳ������е�һ��������ֵ��
		local_max_point[q]=local_temp[0];
		int y1=local_max_point[q].y,x1=local_max_point[q].x;
		double max_index=local_max_point[q].value;
		//����ǰһ����ֵ������������Ӱ��
		for(j=0;j<(height-40)*(width-40);j++)
		{
			int y2=local_temp[j].y,x2=local_temp[j].x;
			temp=-(((y2-y1)*(y2-y1)+(x2-x1)*(x2-x1))/10);
			//ÿ����ֱ��ȥ��˹��������
			local_temp[j].value=local_temp[j].value-max_index*exp(temp);
		}
		q++;
	}
	delete local_temp;
	printf("\n%d\n%d\n",i,q);

	//�Ծֲ���ֵ��ͶӰ��һ���㣬������������ں�
	for(i=0;i<q;i++)
	{
		if(local_max_point[i].value!=0)
		{
			//�ֲ���ֵ�㵱ǰ����ֵ�ĺ��������x*��ǰ��ֵ�����ֵ���ۼӵ�sum_x��
			sum_x+=local_max_point[i].x*local_max_point[i].value;	
			//�ֲ���ֵ�㵱ǰ����ֵ�ĺ��������x*��ǰ��ֵ�����ֵ���ۼӵ�sum_y��
			sum_y+=local_max_point[i].y*local_max_point[i].value;
			//�Ը�����ֵ�����ֵ�����ۼ�
			sum+=local_max_point[i].value;
		}
	}
	//result.valueΪ�����ֲ���ֵ���ƽ����ֵ
	result.value=sum/q;
	//result.xΪ20���ֲ���ֵ��ͶӰΪһ�����x����
	result.x=sum_x/sum;
	//result.xΪ20���ֲ���ֵ��ͶӰΪһ�����y����
	result.y=sum_y/sum;

	printf("%f\t%d%d",result.value,result.x,result.y);
	printf("\n");	

//	cvSaveImage(name,img2);
	
//	cv::namedWindow("image", CV_WINDOW_AUTOSIZE); //��������
//namedWindow("image2", CV_WINDOW_AUTOSIZE); //��������
//	imshow("image", img); //��ʾͼ��
//	imshow("image2", img2); //��ʾͼ��
  
	delete gettemp;
	delete local_max_point;
//	cvDestroyWindow( "image" );//���ٴ���
    cvReleaseImage( &img ); //�ͷ�ͼ��
//	cvDestroyWindow( "image2" );//���ٴ���
    cvReleaseImage( &img2 ); //�ͷ�ͼ��
	return result;
}

bool CCheck::SecondData(DataField *a,DataField tempcheck)//aΪ��������tempcheckΪ�������
{
	DataField temp[8];			//����ͶӰ�ڶ������ݳ������ݵ�����꼰��ֵ
	DataField unsort[8];		//�ڶ������ݳ��У��������8������ǰ7�������������1��Ϊ�������
	for(int i=0;i<8;i++)
	{	
		temp[i].value=0;
		temp[i].x=0;
		temp[i].y=0;
	}
	for(int i=0;i<7;i++)
	{
		temp[i]=a[i];
	}

	temp[7]=tempcheck;
//	m_objCheck.temp1[7]=temp[7];

	double second_datafield [128] [128];	//����128*128�Ķ�ά���飬��Ϊ�������ݳ�
	//�Զ������ݳ����г�ʼ����ʹ��ֵ����0
	for(int i=0;i<128;i++)
		for(int j=0;j<128;j++)
			second_datafield[i][j]=0;

	//�����������ݳ������ÿ�������ֵ
	for(int y=0;y<128;y++)
		for(int x=0;x<128;x++)
			for(int k=0;k<8;k++)
			{
				//dΪ�������ݳ������ݵ�㣨ÿ��ͼ�񣩵����ݳ��е�ǰ��ľ����ƽ��,����sigema=3
				double d=-((temp[k].x-x)*(temp[k].x-x)+(temp[k].y-y)*(temp[k].y-y))/9;	//�����ƽ��							
				second_datafield[x][y]+=temp[k].value*exp(d);
			}
	printf("\n\n\n");
     
	//��ͶӰ���������ݳ��е�8�������������
	for(int i=0;i<8;i++) 
	{	
		unsort[i].number=temp[i].number;
		unsort[i].x=temp[i].x;
		unsort[i].y=temp[i].y;
		unsort[i].value=second_datafield[temp[i].x][temp[i].y];
	}
	BubbleSort(unsort,8);

	//flagΪ��Ⱥ���־λ�����������Ϊ��Ⱥ�㣬flag��true������Ϊfalse
	bool flag=false;
	//������󣬴�����������������ĩβ����˵���ö����ڶ������ݳ�����ֵ��С����Ϊ��Ⱥ��
	for(int i=0;i<8;i++)
	{
		if(unsort[i].number==8&&i==7&&(unsort[6].value-unsort[7].value)<2)
			flag=true;
	}
	return flag;
//	for(int i=0;i<8;i++)
//		printf("%f\t%s%d%s%d%s\t%d\n",unsort[i].value,"(",unsort[i].x,",",unsort[i].y,")",unsort[i].number);


}
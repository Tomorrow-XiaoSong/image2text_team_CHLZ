#include"image2text.h"


Image2Text::Image2Text(string input_image_address)
{
	input_image = imread(input_image_address, IMREAD_UNCHANGED);//打开方式为：IMREAD_UNCHANGED,8位，彩色或非彩色
	if (input_image.empty()) {									 //打开错误时发送错误信息
		cerr << "Error! The input image can't be read...\n";
		exit(0);
	}

}
Image2Text::~Image2Text() {
	delete[] huidu_matrix.matrix;
	delete[] char_image;
}
Image2Text::huiduMatrix Image2Text::RGB_to_huiduMatrix(Mat RGB, int width, int height) 			//返回值为类内声明的结构体
{
	//彩图 转 灰度图片
	Mat huidu_image;
	cvtColor(RGB, huidu_image, COLOR_BGR2GRAY);							//直接调用cvtColor函数，通过第三个参数设置转换方式为彩图转灰度图片

	//未完成——灰度图片压缩：把灰度图片huidu_image压缩为宽度为width,高度为height


	//灰度图片 转 灰度矩阵
	int nl = huidu_image.rows;									//行数  
	int nc = huidu_image.cols * huidu_image.channels();						//列数 = 像素点列数*通道数（此处为灰度图片只有一个gray通道。彩色图片有RGB三个通道）
	huidu_matrix.matrix = new int[nl*nc + 1];							//定义灰度矩阵，用一维数组表示二维数组，便于后边操作
	huidu_matrix.width = nc;									//灰度矩阵宽度赋值
	huidu_matrix.height = nl;									//灰度矩阵高度赋值
	for (int j = 0; j<nl; j++) {
		uchar* data = huidu_image.ptr<uchar>(j);						//data记录图片每行像素的起始地址,ptr<uchar>(j)获得第j行的头指针，<uchar>是图片中的数据类型
		for (int i = 0; i<nc; i++) {								//将该行地址下的所有像素记录到灰度矩阵的对应行中
			huidu_matrix.matrix[j*nc + i] = data[i];
		}
	}
	return huidu_matrix;
}

char* Image2Text::huiduMatrix_to_charImage(const huiduMatrix &huiduMatrix)
{
	//未完成——把灰度矩阵转化为字符画
	cerr << "the function\"huiduMatrix_to_charImage()\" hadn't been finish...\n";
	system("pause");
	exit(0);
	return char_image;
}

char* Image2Text::huiduMatrix_to_nondestructiveImage(const huiduMatrix &huidu_matrix) {
	char huidu_char[] = "@#xo+. ";									//灰度字符
	char_image = new char[huidu_matrix.height*huidu_matrix.width + 1];
	for (int j = 0; j < huidu_matrix.height; j++) {							//根据灰度矩阵的像素值给灰度图片的每个像素匹配字符
		for (int i = 0; i < huidu_matrix.width; i++) {						//0-35:@   36-71:#   72-108:x   109-144:o   145-180:+   181-216:-   217-255: 
			if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 35)char_image[j*huidu_matrix.width + i] = huidu_char[0];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 71)char_image[j*huidu_matrix.width + i] = huidu_char[1];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 108)char_image[j*huidu_matrix.width + i] = huidu_char[2];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 144)char_image[j*huidu_matrix.width + i] = huidu_char[3];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 180)char_image[j*huidu_matrix.width + i] = huidu_char[4];
			else if (huidu_matrix.matrix[j*huidu_matrix.width + i] <= 216)char_image[j*huidu_matrix.width + i] = huidu_char[5];
			else char_image[j*huidu_matrix.width + i] = huidu_char[6];
		}
	}
	return char_image;
}

void Image2Text::to_txt(string output_address, int type_of_outputImage, int width, int height) {
	char * result_image;										//记录生成的字符画矩阵
	int nl, nc;											//记录生成的字符画矩阵的宽度nc，高度nl
	switch (type_of_outputImage) {									//根据用户选择的输出图片类型：无损图片或者字符画图片来输出txt
	case NONDESTRUCTIVEIMAGE:									//txt格式：宋体 常规体 大小1  max500*500
	{
		result_image = huiduMatrix_to_nondestructiveImage(RGB_to_huiduMatrix(this->input_image));
		nl = RGB_to_huiduMatrix(this->input_image).height;
		nc = RGB_to_huiduMatrix(this->input_image).width;
		break;
	}
	case CHARIMAGE:											//txt格式：宋体 黑体 大小8  max70*70
	{
		result_image = huiduMatrix_to_charImage(RGB_to_huiduMatrix(this->input_image, width, height));
		nl = RGB_to_huiduMatrix(this->input_image, width, height).height;
		nc = RGB_to_huiduMatrix(this->input_image, width, height).width;
		break;
	}
	default:											//type_of_outputImage为未定义的格式时，打印错误信息
	{
		cerr << "the type of the outputImage only can be NONDESTRUCTIVEIMAGE or CHARIMAGE.\n";
		system("pause");
		exit(0);
		break;
	}
	}

	//标准化输出文件的命名格式为 xxx.txt
	if (output_address.substr(output_address.length() - 4, 4) != ".txt") {
		string a = output_address.substr(output_address.length() - 5, 4);
		output_address += ".txt";
	}

	//将生成的字符画图片储存\
	未完成——控制.txt字体格式
	ofstream f(output_address, ios::out);
	for (int j = 0; j < nl; j++) {
		for (int i = 0; i < nc; i++) {
			f << char_image[j*nc + i];
			f << char_image[j*nc + i];
		}
		f << '\n';
	}
	f.close();
}


void Image2Text::to_html(string output_address)
{
	//标准化输出文件的命名格式为 xxx.html
	if (output_address.substr(output_address.length() - 5, 5) != ".html") {
	output_address += ".html";
	}
	
	//未完成——输出html
	cerr << "the function hadn't\"to_html\" been finish...\n";
	system("pause");
	exit(0);
};

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

  FILE *infp, *outfp;

  struct IMAGEDATA{
      char magic [8];
      int width;
      int height;
      int max;
      int image[1024][1024];
      int imageout[4096][4096];
  };

  struct IMAGEDATA images;

  //縦横比を変えるための変数を扱う構造体
  struct CHANGEIMAGES{
      double x0 ;
      double y0 ;
      int u;
      int v;
      double alpha;
      double beta;
      double totalData;
      double data1;
      double data2;
      double data3;
      double data4;
  };

  struct CHANGEIMAGES change;


 /*ファイルの読み込み*/
  int readImage(char infile[1024] ){
      /*ファイル読み込み専で開く*/	  

      infp = fopen(infile, "r");
      if(infp == NULL){
        printf("file open error");
        exit(EXIT_FAILURE);
      }
      /*ファイルからのデータ読み込み*/
      fscanf(infp, "%s", images.magic);  
      fscanf(infp, "%d" ,&images.width);
      fscanf(infp, "%d" ,&images.height);
      fscanf(infp, "%d" ,&images.max);


      for(int i = 0; i <images.height; i ++){
          for(int j = 0; j <images.width; j++){
            fscanf(infp, "%d", &images.image[i][j]);          
	  }
      }
     

      fclose(infp);

      return 0;
  }

  



  int changeImage(double a, double b, double c,double d, double e, double f){

      //printf("tempcos = %f tempsin = %f\n", tempcos,tempsin);
    if(a * e != b * d){

      for(int y = 0; y < images.height; y++){
          for(int x = 0; x <images.width; x++ ){
              

 
              change.x0 =  (e /(a * e  -b * d ) * (x - c)) - (b / (a * e - b * d)* (y -f));
              change.y0 =  (d/ (b * d - a * e ) * (x - c)) - (a / (b * d - a * e)* (y -f));

              change.u = (int)change.x0;
              change.v = (int)change.y0;

              //x0,y0と基準u,vの比率を求める
              change.alpha = change.x0 - (double)change.u;
              change.beta = change.y0 - (double)change.v;


              if(change.x0 >= 0 && change.x0 < (double)images.width -1 && change.y0 >= 0 && change.y0 < (double)images.height-1){
                  //線形補間法による補正
                  change.data1 = images.image[change.v][change.u] * (1-change.alpha)*(1-change.beta);
                  change.data2 = images.image[change.v][change.u+1] * change.alpha* (1-change.beta);
                  change.data3 = images.image[change.v+1][change.u]* (1-change.alpha)* change.beta ;
                  change.data4 = images.image[change.v+1][change.u+1]*change.alpha * change.beta;

                  change.totalData = change.data1+ change.data2+ change.data3+ change.data4 ;
                  images.imageout[y][x] = (int)change.totalData;
              }
                  
          }
          
      }
    
    }

  }

 /*ファイルの書き込み*/ 
  int writeImage(char outfile[1024]){
      /*指定のファイル書き込み専で開く*/	  

      outfp = fopen(outfile, "w");

      if(outfp == NULL){
        printf("file open error");
        exit(EXIT_FAILURE);
      }

      /*ファイルへのデータ書き出し*/
      fprintf(outfp,"%s\n", images.magic);
      fprintf(outfp, "%d", images.width);
      fprintf(outfp, "% d\n", images.height);
      fprintf(outfp, "%d\n", images.max);


      for(int i = 0; i < images.height; i++ ){
            for(int j = 0; j < images.width; j++){

              if(j == 9){
              fprintf(outfp, "% d\n", images.imageout[i][j]);
              }
              else{
               fprintf(outfp,"% d", images.imageout[i][j]);
              }

            }
      }
      fclose(outfp);

      return 0;
  }

/*メイン関数*/
int main(int argc, char **argv){

  readImage(argv[1]);
  double a = atof(argv[3]);
  double b = atof(argv[4]);
  double c = atof(argv[5]);
  double d = atof(argv[6]);
  double e = atof(argv[7]);
  double f = atof(argv[8]);
  
  changeImage(a, b, c, d, e, f);

  writeImage(argv[2]);

}

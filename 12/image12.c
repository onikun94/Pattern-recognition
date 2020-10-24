#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include<limits.h>

  FILE *infp, *outfp;

  struct IMAGEDATA{
      char magic [8];
      int width;
      int height;
      int max;
      int image[1024][1024];
      int a[9];
      int imageout[1024][1024];

  };

  struct IMAGEDATA images;


  struct LABEL{
      int dataLabel;
      int maxLabel;
  };

  struct LABEL lab;

  struct JUDGEDATA{
      double n[512];//画素値iを持つ画素の頻度
      double N;//全画素数
      double p[512];//画素の確率
      double omega;//ω
      double micro;//μ
      double microT;//μT
      double dispersion;//クラス間分散
      double max;//クラス間分散の最大値
      double threshold;//閾値

  };

  struct JUDGEDATA judge;

  struct LABELDATA{
      int area;
      int clusterNum;
      //int totalX;
      //int totalY;
      //int barycenterX;
      //int barycenterY;
  };

  struct LABELDATA labels[1024];

  struct CLUSTERDATA{
      int center;
      int preCenter;
      int patternNum;
      int Totalarea;
  };

  struct CLUSTERDATA clus[1024];

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


  //ωを再帰的に求める
  double askOmega(int k){
      if (k == 0){
        return judge.p[k];
      }
      return askOmega(k-1) + judge.p[k];
  }
  //μを再帰的に求める
  double askMicro(int k)
  {
      if(k == 0){
        return k * judge.p[k];
      }
      return askMicro(k-1) + k * judge.p[k]; 
      
  }

  //クラス間分散を求める
  int distributedClasses(){

    //niを求める
    for(int k = 0; k <= images.max; k++){
        judge.n[k] = 0; 
    }

    for(int i = 0; i < images.height; i++){
        for(int j = 0; j < images.width; j++){
            judge.n[images.imageout[i][j]]++;
        }
    }

    //piを求める
    judge.N = images.width * images.height;

    for (int k = 0; k <= images.max; k++){
        judge.p[k] = judge.n[k] / judge.N;
    }

    //microTを求める
    for(int t = 0; t <= images.max; t++){
        judge.microT += t * judge.p[t];
    }

    //各kの値によってクラス間分散を求めてクラス間分散が最大のときのkを求める
    for(int k = 0; k < images.max; k++){
        judge.omega = askOmega(k);
        judge.micro = askMicro(k);
        
        //分母が0のときの処理
        if(judge.omega == 1 || judge.omega == 0){
            continue;
        }

        //クラス間分散を求める
        judge.dispersion = pow((judge.microT * judge.omega - judge.micro), 2.0) / (judge.omega* (1- judge.omega));
        
        //クラス間分散の最大値を求め,その時の閾値を求める
        if(judge.dispersion > judge.max){
            judge.max = judge.dispersion;
            judge.threshold = k;
        }
    }

  }

//ラベリング処理
int addLabel(int i, int j, int label)
  {
      images.imageout[i][j] = label;
      if (images.imageout[i - 1][j - 1] == images.max)
      {
          addLabel(i - 1, j - 1, label);
      }
      if (images.imageout[i - 1][j] == images.max)
      {
          addLabel(i - 1, j, label);
      }
      if (images.imageout[i - 1][j + 1] == images.max)
      {
          addLabel(i - 1, j + 1, label);
      }
      if (images.imageout[i][j - 1] == images.max)
      {
          addLabel(i, j - 1, label);
      }
      if (images.imageout[i][j + 1] == images.max)
      {
          addLabel(i, j + 1, label);
      }
      if (images.imageout[i + 1][j - 1] == images.max)
      {
          addLabel(i + 1, j - 1, label);
      }
      if (images.imageout[i + 1][j] == images.max)
      {
          addLabel(i + 1, j, label);
      }
      if (images.imageout[i + 1][j + 1] == images.max)
      {
          addLabel(i + 1, j + 1, label);
      }
  }

  //すべての場所にラベリング処理
  int allLabel(){
    //ラベリング
    int label = 1;
    for(int i = 0; i< images.height; i++){
        for(int j = 0; j < images.width; j++){
            if(images.imageout[i][j] == images.max){
                addLabel(i, j, label);
                label ++;
            }
            
        }
    }
    lab.dataLabel = label;
  }

  //ラベルの面積を求める
  int askLabelArea(){
    allLabel();
      //labelの面積を求めていく
      //初期化
     for(int i = 0; i < lab.dataLabel; i++ ){
          labels[i].area = 0; 
      }

      //labelごとの面積を求める
      for(int y = 0; y < images.height; y ++){
          for(int x = 0; x < images.width; x ++){

              labels[images.imageout[y][x]].area ++;
          }
      }
      

  }
  /*クラスタリング*/
  int clustering(int clusterNumber){

      //クラスタの最初の中心を定める
      for(int i = 0; i < clusterNumber; i++){
          clus[i].center = labels[i].area;
      }

      int clusterDiff = 0;
      do
      {
          //初期化
          clusterDiff = 0;
          for (int p = 0; p < lab.dataLabel; p++)
          {
              clus[labels[p].clusterNum].patternNum = 0;
              clus[labels[p].clusterNum].Totalarea = 0;
          }
          printf("clasterDiff = %d\n", clusterDiff);

          for(int p = 0; p < lab.dataLabel; p++){
              int minDis = INT_MAX;
              for(int i = 0; i< clusterNumber; i++){
                  int distance = abs(clus[i].center - labels[p].area);
                  if(distance < minDis){
                      minDis = distance;
                      labels[p].clusterNum = i;
                  }
              }
            clus[labels[p].clusterNum].patternNum++;
            clus[labels[p].clusterNum].Totalarea += labels[p].area;

            printf("patternnum = %d\n",clus[labels[p].clusterNum].patternNum);
            printf("totalarea = %d\n",clus[labels[p].clusterNum].Totalarea);
          }

          
          for(int i = 0; i < clusterNumber; i++){
              clus[i].preCenter = clus[i].center;
              clus[i].center = clus[i].Totalarea / clus[i].patternNum;
              clusterDiff +=  abs(clus[i].center -clus[i].preCenter); 
          }
          printf("clasterDiff = %d\n", clusterDiff);
          printf("----------------\n");

      } while (clusterDiff> 0);
      
      return 0;
  }

  
  //すべての画像処理に関する関数
  int changeImage(int number){

    //反転処理
    for(int y = 0; y < images.height; y++){
        for(int x = 0; x < images.width; x++){
            images.imageout[y][x] = images.max - images.image[y][x];
        }
    }

    //二値化処理
    distributedClasses();
    
    for(int i = 0; i < images.height; i++){
        for(int j = 0; j < images.width; j++){
            if(images.imageout[i][j] > judge.threshold){
                images.imageout[i][j] = images.max;
            }
            else{
                images.imageout[i][j] = 0;
            } 
        }
    }
    //ラベル処理
    askLabelArea();

    //クラスタリング処理
    clustering(number);

    //出力
    for(int i = 0; i < lab.dataLabel; i++){

         printf("labelNumber = %d,  area = %d clusterNumber = %d\n",i, labels[i].area, labels[i].clusterNum);
      }

    return 0;
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
  int clNumber = atoi(argv[3]);
  changeImage(clNumber);
  writeImage(argv[2]);
}

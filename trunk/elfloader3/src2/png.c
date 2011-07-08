#include "loader3\loader.h"
#include <inc/pnglist.h>

extern unsigned int DEFAULT_COLOR;
extern unsigned int ALPHA_THRESHOLD;
extern unsigned int CACHE_PNG;
extern unsigned int DEFAULT_DISK_N;

const char DEFAULT_FOLDER[]=":\\ZBin\\img\\";

#define number 8

const char Pointer[1]={0xFF};
const IMGHDR empty_img = {0,0,0x1,(char *)Pointer};

void* xmalloc(int x,int n)
{
  return malloc(n);
}

void xmfree(int x,void* ptr)
{
  mfree(ptr);
}

__arm void read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
  unsigned int err;
  int f;
  f=(int)png_get_io_ptr(png_ptr);
  fread(f, data, length, &err);
}

__arm IMGHDR* create_imghdr(const char *fname, int type)
{
  int f;
  char buf[number];
  unsigned int err;
  struct PP
  {
    char *row;
    char *img;
    IMGHDR * img_h;
  } pp;
  IMGHDR * img_hc;
  png_structp png_ptr=NULL;
  png_infop info_ptr=NULL;
  png_uint_32 rowbytes;
  
  if ((f=fopen(fname, A_ReadOnly+A_BIN, P_READ, &err))==-1) return 0;
  pp.row=NULL;
  pp.img=NULL;
  pp.img_h=NULL;
  
  if (fread(f, &buf, number, &err)!=number) goto L_CLOSE_FILE;
  if  (!png_check_sig((png_bytep)buf,number)) goto  L_CLOSE_FILE;
  
  png_ptr = png_create_read_struct_2("1.2.5", (png_voidp)0, 0, 0, (png_voidp)0,(png_malloc_ptr)xmalloc,(png_free_ptr)xmfree);
  if (!png_ptr) goto L_CLOSE_FILE;
  
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    goto L_CLOSE_FILE;
  }
  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    goto L_CLOSE_FILE;
  }
  
  png_set_read_fn(png_ptr, (void *)f, read_data_fn);
  
  png_set_sig_bytes(png_ptr, number);
  
  png_read_info(png_ptr, info_ptr);
  
  png_uint_32 width, height;
  int bit_depth, color_type;
  
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
  
  if (type==0)
  {
    if (color_type == PNG_COLOR_TYPE_GRAY) 
      type=PNG_1;
    else type=DEFAULT_COLOR+1;
  }
  
  if (bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
    
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr); 
  
  if (bit_depth == 16) png_set_strip_16(png_ptr);
  
  if (bit_depth < 8) png_set_packing(png_ptr);
  
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);
  
  if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_GRAY)
    png_set_gray_to_rgb(png_ptr);
  
  png_set_filler(png_ptr,0xFF,PNG_FILLER_AFTER);
  png_read_update_info(png_ptr, info_ptr);
  
  rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  
  pp.row=malloc(rowbytes);
  pp.img_h=img_hc=malloc(sizeof(IMGHDR));
  
  if (type==PNG_1)
  {
    int rowc_w=(width+7)>>3;
    int size=height*rowc_w;
    unsigned char *iimg=(unsigned char *)(pp.img=malloc(size));
    zeromem(iimg,size);
    for (unsigned int y = 0; y<height; y++)
    {
      png_read_row(png_ptr, (png_bytep)pp.row, NULL);
      for (unsigned int x = 0; x<width; x++)
      {
        if (!pp.row[x*4+0] && !pp.row[x*4+1] && !pp.row[x*4+2])
          iimg[x>>3]|=(0x80>>(x&7));
      }
      iimg+=rowc_w;
    }
    pp.img_h->bpnum=1;
  }
  else
  {
    switch (type)
    {
    case PNG_8:
      {
        unsigned char *iimg=(unsigned char *)(pp.img=malloc(width*height));
        for (unsigned int y = 0; y<height; y++)
        {
          png_read_row(png_ptr, (png_bytep)pp.row, NULL);
          for (unsigned int x = 0; x<width; x++)
          {
            if (pp.row[x*4+3]<ALPHA_THRESHOLD)
              *iimg++=0xC0;
            else
            {
              unsigned char c=(pp.row[x*4+0] & 0xE0);
              c|=((pp.row[x*4+1]>>3)&0x1C);
              c|=((pp.row[x*4+2]>>6)&0x3);
              *iimg++=c;
            }
          }
        }
        pp.img_h->bpnum=5;
        break;
      }
    case PNG_16:
      {
        unsigned short *iimg=(unsigned short *)(pp.img=malloc(width*height*2));
        for (unsigned int y = 0; y<height; y++)
        {
          png_read_row(png_ptr, (png_bytep)pp.row, NULL);
          for (unsigned int x = 0; x<width; x++)
          {
            if (pp.row[x*4+3]<ALPHA_THRESHOLD)
              *iimg++=0xE000;
            else
            {
              unsigned int c=((pp.row[x*4+0]<<8)&0xF800);
              c|=((pp.row[x*4+1]<<3)&0x7E0);
              c|=((pp.row[x*4+2]>>3)&0x1F);
              *iimg++=c;
            }
          }
        }
        pp.img_h->bpnum=8;
        break;
      }
#if NEWSGOLD || X75
    case PNG_24:
      {
        unsigned char *iimg=(unsigned char *)(pp.img=malloc((width*height)<<2));
        for (unsigned int y = 0; y<height; y++)
        {
          png_read_row(png_ptr, (png_bytep)pp.row, NULL);
          for (unsigned int x = 0; x<width; x++)
          {
	    unsigned int c;
            *iimg++=pp.row[x*4+2];
            *iimg++=pp.row[x*4+1];
            *iimg++=pp.row[x*4+0];
	    c=pp.row[x*4+3];
//	    if (c>=128) c++;
//	    c*=100;
//	    c>>=8;
            *iimg++=c;//(pp.row[x*4+3]*100)/0xFF;
          }
        }
        pp.img_h->bpnum=0xA;
        break;
      }
#endif
    }
  }
  pp.img_h->w=width;
  pp.img_h->h=height;
  //pp->img_h->zero=0;
  pp.img_h->bitmap=pp.img;
  
  png_read_end(png_ptr, info_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  if (!pp.img)
  {
  L_CLOSE_FILE:
    mfree(pp.row);
    mfree(pp.img);
    mfree(pp.img_h);
    fclose(f, &err);
    return NULL;
  }
  mfree(pp.row);
  fclose(f, &err);
  return (img_hc);
}

volatile __no_init PNGTOP_DESC pngtop; 

#pragma inline
int tolower(int C)
{
  if ((C>='A' && C<='Z')) C-='A'-'a';
  return(C);
}

char* strcpy_tolow(char *s1,const char *s2)
{
  while(*s2)
  {
    *s1++=tolower(*s2++);   
  }
  *s1=0;
  return s1;
}

__arm void print10(char *s, unsigned int v)
{
  unsigned int buf=0xF;
  while(v>=10)
  {
    extern int __e_div(int delitelb, int delimoe);
    buf=(buf<<4)|(__e_div(10, v)/*v%10*/);
    v = udiv(10, v); //v/=10;
  }
  *s++=v+'0';
  while((v=buf&0x0F)<10) {*s++=v+'0'; buf>>=4;}
  *s++='.';
  *s++='p';
  *s++='n';
  *s++='g';
  *s='\0';
}

__arm IMGHDR *find_png_in_cache(char *png_name)
{
  PNGLIST *pl;
  PNGLIST *pl_prev;
  LockSched();
  pl=(PNGLIST *)(&(pngtop.pltop));
  pl_prev=NULL;  
  while((pl=pl->next))
  {
    if (!strcmp(pl->pngname,png_name))
    {
      //Найден, переносим в начало и выходим
      if (pl_prev)
      {
	//Только если не в самом начале
	pl_prev->next=pl->next; //Удалили из найденого места
	pl->next=(PNGLIST *)(pngtop.pltop); //Следующий - весь список
	pngtop.pltop=pl; //А первый в списке - найденый
      }
      UnlockSched();
      return(pl->img);
    }
    pl_prev=pl; //Текущий обработанный - теперь предыдущий
  }
  UnlockSched();
  return (0);
}

__arm IMGHDR* PatchGetPIT(unsigned int pic)
{
  IMGHDR * img;
  unsigned int i;
  char fname[256];
  
  PNGLIST *pl;
  PNGLIST *pl_prev;
  PNGLIST *cur;
  unsigned int mask80;
  unsigned int mask40;
  char *bp;
  if ((pic>>28)==0xA)
  {
    strcpy_tolow(fname,(char*)pic);
    img=find_png_in_cache(fname);
    if (img) return (img);
    img=create_imghdr(fname,0);
    if (!img) return ((IMGHDR *)&empty_img);
  }
  else
  {
    //Ищем в списке динамических иконок
    {
      DYNPNGICONLIST *dynp;
      LockSched();
      dynp=pngtop.dyn_pltop;
      while(dynp)
      {
	if (dynp->icon==pic)
	{
	  IMGHDR *i=dynp->img;
	  if (i)
	  {
	    UnlockSched();
	    return(i);
	  }	  
	}
	dynp=dynp->next;
      }
      UnlockSched();
    }
    if ((pngtop.bitmap) && (pic<20000))
    {
      mask40=(mask80=0x80UL>>((pic&3)<<1))>>1;
      bp=pngtop.bitmap+(pic>>2);
      if ((i=*bp)&mask80)  // Есть запись в битмапе
      {
        if (i&mask40)  
        {
          char *next=strcpy_tolow(fname+1,DEFAULT_FOLDER); // Картинка вроде как есть на диске
	  *fname=DEFAULT_DISK_N+'0';
          print10(next,pic);
          img=find_png_in_cache(fname);
          if (img) return (img);
          img=create_imghdr(fname,0);          
        } 
        else return(0);                                // Картинки нет - выходим
      }
      else 
      {
	LockSched();
	*bp|=mask80; // Записи нет, ставим флаг что есть
	UnlockSched();
        char *next=strcpy_tolow(fname+1,DEFAULT_FOLDER);
	*fname=DEFAULT_DISK_N+'0';
        print10(next,pic);
        img=find_png_in_cache(fname);
        if (img)
        {
	  LockSched();
	  *bp|=mask40;
	  UnlockSched();
          return (img);
        }
        img=create_imghdr(fname,0);                 // Пробуем создать
        if (img)
        {
	  LockSched();
	  *bp|=mask40;
	  UnlockSched();
        }
        else  return (0);
      }
    }
    else return(0);
  }
  //Ничего не нашли, теперь пробуем добавить
  
  //if (!img) return (0); //Нечего добавлять
  cur=malloc(sizeof(PNGLIST)); //Создаем элемент списка
  cur->pngname=malloc(strlen(fname)+1);
  strcpy(cur->pngname,fname);
  cur->img=img;
  i=0; //Это количество элементов в списке
  LockSched();
  cur->next=(PNGLIST *)(pngtop.pltop); //Следующий - весь список
  pngtop.pltop=cur; //Первый в списке - новый элемент
  //Теперь подрезаем конец
  pl=(PNGLIST *)(&(pngtop.pltop));
  do
  {
    pl_prev=pl;
    pl=pl->next;
    if (!pl)
    {
      //Закончились элементы раньше
      UnlockSched();
      return(cur->img);
    }
    i++;
  }
  while(i<=CACHE_PNG); //Пока количество элементов меньше допустимого
  pl_prev->next=NULL; //Обрежем список
  UnlockSched();
  //Остальное можно сделать с разлоченной многозадачностью
  do
  {
    //Удаляем текущий
    if (pl->img)
    { 
      mfree(pl->img->bitmap);
      mfree(pl->img);
    }
    mfree(pl->pngname);
    pl_prev=pl;
    pl=pl->next;
    mfree(pl_prev);
  }
  while(pl); //Пока есть элементы, освобождаем их
  return(cur->img);
}

__arm void InitPngBitMap(void)
{
  if (!pngtop.bitmap)
  {
    pngtop.bitmap=malloc(20000/8*2);
  }
  zeromem((void*)(pngtop.bitmap),20000/8*2);
}

#pragma diag_suppress=Pe177
__root static const int SWILIB_FUNC1E9 @ "SWILIB_FUNC1E9" = (int)create_imghdr;
#pragma diag_default=Pe177

#include "wavefile.h"

void printHeader(HeaderType header) {
	printf("  ChunkID:       %x\n", header.ChunkID);
	printf("  ChunkSize:     %d\n", header.ChunkSize);
	printf("  Format:        %x\n", header.Format);
	printf("  Subchunk1ID:   %d\n", header.Subchunk1ID);
	printf("  Subchunk1Size: %d\n", header.Subchunk1Size);
	printf("  AudioFormat:   %d\n", header.AudioFormat);
	printf("  NumChannels:   %d\n", header.NumChannels);
	printf("  SampleRate:    %d\n", header.SampleRate);
	printf("  ByteRate:      %d\n", header.ByteRate);
	printf("  BlockAlign:    %d\n", header.BlockAlign);
	printf("  BitsPerSample: %d\n", header.BitsPerSample);
	printf("  Subchunk2ID:   %x\n", header.Subchunk2ID);
	printf("  Subchunk2Size: %d\n", header.Subchunk2Size);
}

/*
 *  Discrete Cosine Transform
 *
 *    Aplica a transformada discreta do cosseno em um vetor de dados.
 *
 *    sample: vetor de dados
 *    result: vetor resultante da aplicacao da dct
 *    n: tamanho do vetor de dados 
 *
 */
void dct(short int *sample, short int *result, int n) {
	int    i, u, Cu;
	double sum;
	
	for (u = 0;u < n;u++) {
		Cu = 1.;
		if (u == 0) Cu = (sqrt(2) / 2.);
	
		sum = 0.0;
		for (i = 0;i < n;i++)
			sum = sum + cos(((2.*i + 1.)*M_PI*u) / (2.*((float) n))) * 
				((float) sample[i]);
		result[u] = (int) (sum * (Cu / 2.));
	}
}

/*
 *  Inverse Discrete Cosine Transform
 *
 *    Aplica a transformada inversa do cosseno em um vetor de dados.
 *
 *    sample: vetor de dados
 *    result: vetor resultante da aplicacao da dct
 *    n: tamanho do vetor de dados 
 *
 */
void idct(short int *sample, short int *result, int n) {
	int    i, u, Ci;
	double sum;
	
	for (u = 0;u < n;u++) {	
		sum = 0.0;
		for (i = 0;i < n;i++) {
			Ci = 1.;
			if (i == 0) Ci = (sqrt(2) / 2.);
			
			sum = sum + (cos(((2.*u + 1.)*M_PI*i) / (2.*((float) n)) * 
				((float) sample[i])) * (Ci / 2.));
		}
		result[u] = (int) sum;
	}
}

/*
 *  Run-Length Encoding
 *
 *    Aplica a RLE em um vetor de dados.
 *
 *    sample: vetor de dados
 *    result: vetor resultante da aplicacao da rle
 *    n: tamanho do vetor de dados 
 *
 */
int rle(short int *sample, short int *result, int n) {
	int i, j, count;
	
	// solucao do protocolo MNP5
	
	result[0] = sample[0];
	count = 1;
	
	for (i = 1, j = 1;i < n;i++) {
		if (sample[i-1] == sample[i]) {
			count++;
			if (count < 4)
				result[j++] = sample[i];
		} else {
			if (count > 2)
				result[j++] = (short int) (count - 3);
			result[j++] = sample[i];
			count = 1;
		}
	}
	
	if (count > 2)
		result[j++] = (short int) (count - 3);
	
	return j;
}

/*
 *  Run-Length Encoding
 *
 *    Recupera da RLE em um vetor de dados.
 *
 *    sample: vetor de dados
 *    result: vetor resultante da aplicacao da rle
 *    n: tamanho do vetor de dados 
 *
 */
int unrle(short int *sample, short int *result, int n) {
	int i, j, k, count;
	
	// solucao do protocolo MNP5
	
	result[0] = sample[0];
	count = 1;
	
	for (i = 1, j = 1;i < n;i++) {
		if (count > 2) {
			for (k = 0;k < sample[i];k++)
				result[j++] = sample[i-1];
			if (i+1 < n) {
				result[j++] = sample[++i];
				count = 1;
			}
		} else {
			if (sample[i-1] == sample[i]) count++;
			result[j++] = sample[i];
		}
	}
			
	return j;
}

/*
 *  SNR - Signal Noise Rate
 */
int snr(short int *s, short int *o, int n) {
	int  t, j, r;
	int  sum_s, sum_o;
		
	sum_s = 0;
	for (t = 0;t < n;t++) sum_s = sum_s + s[t]*s[t];
	sum_o = 0;
	for (t = 0;t < n;t++) sum_o = sum_o + (s[t]-o[t])*(s[t]-o[t]);
	
	return (int) 10*log10((int) (sum_s / sum_o));
}

void readWave(char *filename, HeaderType *header, WaveData *data) {
  FILE       *f;
  size_t     result;
  char       wfilename[255];

  f = fopen(filename,"rb");
  if (f != NULL) {  
    result = fread(header, 1 , sizeof(HeaderType), f);
	
	if (result == sizeof(HeaderType)) {
		data->Sample      = (short int *) malloc(header->Subchunk2Size);
		result           = fread(data->Sample, 1 , header->Subchunk2Size, f);
				
		if (result != header->Subchunk2Size) {
			printf("Error. Readed data size is different from Subchunk2Size.");
			// quando a quantidade de bytes lida for diferente de Subchunk2Size
			// troca valor de Subchunk2Size
			header->Subchunk2Size = result;
		}
	}
	
    fclose(f);
  } else {
	printf("Couldn't open the file.\n");
  }
}

void writeWave(char *filename, HeaderType *header, WaveData *data) {
  FILE *f;

  f = fopen(filename, "wb");
  
  fwrite(header, 1, sizeof(HeaderType), f);
  fwrite(data->Sample, 1, header->Subchunk2Size, f);

  fclose(f);	
}

/* 
 *	Funcao para Adicionar Ruido
 *
 *    Adiciona ruido no sinal e salva em um arquivo.
 *
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 *
 */
void adicionarRuido(HeaderType header, WaveData data, WaveData *ruido) {
	int      i, bps, rnd;
	char     *buffer1, *buffer2;
	int      n;
  
	srand(9999);

	ruido->Sample = (short int *) malloc((int) header.Subchunk2Size);

	if (header.BitsPerSample == 8) {
		buffer1 = (char *) data.Sample;
		buffer2 = (char *) malloc((int) header.Subchunk2Size);
	
		for (i = 0;i < header.Subchunk2Size;i++) {
			rnd = rand() % RND_MAX;
			buffer2[i] = buffer1[i] + rnd;
		}
		ruido->Sample = (short int *) buffer2;
	} else {
		for (i = 0;i < header.Subchunk2Size / 2;i++) {
			rnd = rand() % RND_MAX;
			ruido->Sample[i] = data.Sample[i] + rnd; 
		}
	}         
	
	writeWave("saida_ruido.wav", &header, ruido);
}

/* 
 *	Funcao para Minimizar Ruido
 *
 *    Faz a media simples entre n amostras na janela a atribui o valor da media
 *  a amostra do meio. Salva em um arquivo.
 *
 *	n: numero de amostras utilizadas na media
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 *
 */
void minimizarRuido(int n, HeaderType header, WaveData data) {
	WaveData d_data;
	int      i, j, sum;
	char     *buffer1, *buffer2;
	
	d_data.Sample = (short int *) malloc((int) header.Subchunk2Size);
	
	if (header.BitsPerSample == 8) {
		buffer1 = (char *) data.Sample;
		buffer2 = (char *) malloc((int) header.Subchunk2Size);
		memcpy(buffer2, buffer1, header.Subchunk2Size);
	
		for (i = 0;i + n < header.Subchunk2Size;i++) {
			sum = 0;
			for (j = i;j < i + n;j+=header.NumChannels) {
				sum += buffer1[j];
			}
			buffer2[i+(n/2)] = sum / n;
		}
		d_data.Sample = (short int *) buffer2;
	} else {
		d_data.Sample = data.Sample;
		for (i = 0;i + n < header.Subchunk2Size / 2;i++) {
			sum = 0;
			for (j = i;j < i + n;j+=header.NumChannels) {
				sum += data.Sample[j];
			}
			d_data.Sample[i+(n/2)] = sum / n;
		}
	}
	
	writeWave("saida_minim.wav", &header, &d_data);
}

/* 
 *	Funcao para Minimizar Ruido usando analise do dominio de Fourier
 *
 *    Se baseia na analise do dominio de Fourier para minimizar o ruido. Salva 
 *  em um arquivo.
 *
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 *
 */
void minRuidoFourier(HeaderType header, WaveData data) {

}

/* 
 * Funcao para Alterar Frequencia de Amostragem ex13_l1
 *
 *    Altera a frequencia do sinal sem mudar o cabecalho e salva o arquivo.
 *
 *  fat: fator de modificacao da frequencia
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 *
 */
void alterarFrequencia(float fat, HeaderType header, WaveData data) {
	HeaderType d_header;
	WaveData   d_data;
	int        i, j, bps;
	char       *buffer1, *buffer2;
	
	d_header = header;
	
	d_data.Sample = (short int *) malloc((int) d_header.Subchunk2Size);
	
	buffer1 = (char *) data.Sample;
	buffer2 = (char *) malloc((int) d_header.Subchunk2Size);
	
	// BytesPerSample
	bps = header.NumChannels * (d_header.BitsPerSample / 8);
	
	for (i = 0;fat*i < d_header.Subchunk2Size;i+=bps) {
		for (j = 0;j < bps;j++) {
			buffer2[i+j] = buffer1[((int) (fat*i))+j];
		}
	}
	
	d_data.Sample = (short int *) buffer2;
	
	writeWave("saida_freq.wav", &d_header, &d_data);
}

/* 
 *	Funcao para Decimacao
 *
 * 	fat: fator de decimacao
 * 	filename: nome do arquivo a ser gerado
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 *
 */
void decimar(int fat, HeaderType header, WaveData data) {
	HeaderType d_header;
	WaveData   d_data;
	int        i, j, bps;
	char       *buffer1, *buffer2;
	char       filename[255], strfat[3]; /* para o filename */
	
	d_header = header;

	d_header.SampleRate     = header.SampleRate / fat;
	d_header.ByteRate       = d_header.SampleRate * header.NumChannels * header.BitsPerSample / 8;
	d_header.Subchunk2Size  = header.Subchunk2Size / fat;
	
	d_data.Sample = (short int *) malloc((int) d_header.Subchunk2Size);
	
	buffer1 = (char *) data.Sample;
	buffer2 = (char *) malloc((int) d_header.Subchunk2Size);
	
	// BytesPerSample
	bps = header.NumChannels * (d_header.BitsPerSample / 8);
	
	/* cria o nome do arquivo */
	sprintf(strfat, "%d", fat);
	strcpy(filename, "saida_dec_");
	strcat(filename, strfat);
	strcat(filename, ".wav");
	
	for (i = 0;i < d_header.Subchunk2Size;i+=bps) {
		for (j = 0;j < bps;j++) {
			buffer2[i+j] = buffer1[fat*i+j];
		}
	}
	
	d_data.Sample = (short int *) buffer2;
	
	writeWave(filename, &d_header, &d_data);
}

/* 
 *	Funcao para Codificar DPCM
 *
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 * 	enc_data: struct dos dados resultantes da codificacao DPCM
 *
 */
void encode_dpcm(HeaderType header, WaveData data, WaveData *enc_data) {
	int i, n;

	enc_data->Sample =  (short int *) malloc((int) n);
	
	/* convencao: manter a primeira amostra e guardar as diferencas p/ amostras 
	 subsequentes */
	if (header.BitsPerSample == 16)
		n = header.Subchunk2Size / 2;
	else
		n = header.Subchunk2Size;
	
	enc_data->Sample[0] = data.Sample[0];
	for (i = 1;i < n;i++)
		enc_data->Sample[i] = data.Sample[i-1] - data.Sample[i];

	writeWave("saida_enc.wav", &header, enc_data);
}

/* 
 *	Funcao para Decodificar DPCM
 *
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 * 	dec_data: struct dos dados resultantes da decodificacao DPCM
 *
 */
void decode_dpcm(HeaderType header, WaveData data, WaveData *dec_data) {
	int i, n;
	
	dec_data->Sample =  (short int *) malloc((int) header.Subchunk2Size);
	/* convencao: manter a primeira amostra e guardar as diferencas p/ amostras 
	 subsequentes */
	if (header.BitsPerSample == 16)
		n = header.Subchunk2Size / 2;
	else
		n = header.Subchunk2Size;
	 
	dec_data->Sample[0] = data.Sample[0];
	for (i = 1;i < n;i++)
		dec_data->Sample[i] = dec_data->Sample[i-1] - data.Sample[i];
	
	writeWave("saida_dec.wav", &header, dec_data);
}

/*
 *  Discrete Cosine Transform
 *
 *    Aplica a transformada discreta do cosseno no arquivo WAVE.
 *
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 * 	dct_data: struct dos dados resultantes da aplicacao da DCT
 *
 */
void aplicarDCT(HeaderType header, WaveData data, WaveData *dct_data) {
	int       n;
	
	dct_data->Sample = (short int *) calloc(header.Subchunk2Size, 
		sizeof(short int));
	
	if (header.BitsPerSample == 16)
		n = header.Subchunk2Size / 2;
	else
		n = header.Subchunk2Size;
		
	dct(data.Sample, dct_data->Sample, n);
	
	writeWave("saida_dct.wav", &header, dct_data);
}

/*
 *  Inverse Discrete Cosine Transform
 *
 *    Aplica a transformada inversa do cosseno no arquivo WAVE.
 *
 * 	header: struct do cabecalho wave
 * 	data: struct dos dados
 * 	dct_data: struct dos dados resultantes da aplicacao da IDCT
 *
 */
void aplicarIDCT(HeaderType header, WaveData dct_data, WaveData *idct_data) {
	int       n;
	
	idct_data->Sample = (short int *) calloc(header.Subchunk2Size, 
		sizeof(short int));
	
	if (header.BitsPerSample == 16)
		n = header.Subchunk2Size / 2;
	else
		n = header.Subchunk2Size;
	
	idct(dct_data.Sample, idct_data->Sample, n);
				
	writeWave("saida_idct.wav", &header, idct_data);
}

/* 
 * Run-Length Encode
 *
 */
int aplicarRLE(HeaderType header, WaveData data, WaveData *rle_data) {
	HeaderType h;
	int        n, t;
	short int  *res;
	
	h = header;
	t = (int) ((h.Subchunk2Size * 4) / 3);
	
	res = (short int *) calloc(t, sizeof(short int));
	
	if (header.BitsPerSample == 16)
		n = header.Subchunk2Size / 2;
	else
		n = header.Subchunk2Size;
	
	t = rle(data.Sample, res, n);
	
	rle_data->Sample = (short int *) calloc(t, sizeof(short int));
	memcpy(rle_data->Sample, res, t);
				
	writeWave("saida_rle.wav", &h, rle_data);
	free(res);
	
	return t;
}

/* 
 * Run-Length Encode
 *
 */
int aplicarUnRLE(HeaderType header, WaveData data, WaveData *unrle_data, int tam) {
	HeaderType h;
	int        n, t;
	short int  *res;
	
	h = header;
	
	res = (short int *) calloc(h.Subchunk2Size, sizeof(short int));
		
	t = unrle(data.Sample, res, tam);
	
	unrle_data->Sample = (short int *) calloc(h.Subchunk2Size, 
		sizeof(short int));
	memcpy(unrle_data->Sample, res, h.Subchunk2Size);
	
	writeWave("saida_unrle.wav", &h, unrle_data);
	free(res);
	
	return t;
}

/* 
 * SNR ex34_l1
 *
 */
int calcularSNR(HeaderType header, WaveData data, WaveData ruido) {
	int n;

	if (header.BitsPerSample == 16)
		n = header.Subchunk2Size / 2;
	else
		n = header.Subchunk2Size;
		
	return snr(data.Sample, ruido.Sample, n);
}

/* 
 * Variar a amplitude (volume) 
 */
void mudarVolume(int fat, HeaderType header, WaveData data, WaveData *vol_data) {
	int i, n;

	if (header.BitsPerSample == 16)
		n = header.Subchunk2Size / 2;
	else
		n = header.Subchunk2Size;
			
	vol_data->Sample = (short int *) malloc((int) header.Subchunk2Size);
		
	for (i = 0;i < n;i++)
//		vol_data->Sample[i] = pow(10, fat) * data.Sample[i];
		vol_data->Sample[i] = 5 * data.Sample[i];
		
	writeWave("saida_vol.wav", &header, vol_data);
}

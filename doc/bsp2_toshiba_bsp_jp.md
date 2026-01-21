# μT-Kernel 3.0 BSP2 ユーザーズマニュアル  <!-- omit in toc -->
## TOSHIBA BSP編  <!-- omit in toc -->
## Version 01.00.00 <!-- omit in toc -->
## 2025.11.26  <!-- omit in toc -->

- [1. はじめに](#1-はじめに)
  - [1.1. 対象マイコンボード](#11-対象マイコンボード)
  - [1.2. 開発環境](#12-開発環境)
  - [1.3. ソフトウェアの構成](#13-ソフトウェアの構成)
- [2. BSP固有の機能について](#2-bsp固有の機能について)
  - [2.1. デバッグ用シリアル出力](#21-デバッグ用シリアル出力)
  - [2.2. 標準ヘッダファイルの使用](#22-標準ヘッダファイルの使用)
  - [2.3. FPU対応](#23-fpu対応)
- [3. デバイスドライバ](#3-デバイスドライバ)
  - [3.1. サンプル・デバイスドライバ(A/DC)](#31-サンプルデバイスドライバadc)
    - [3.1.1. 概要](#311-概要)
    - [3.1.2. デバイスドライバの使用方法](#312-デバイスドライバの使用方法)
  - [3.2. サンプル・デバイスドライバ(I2C)](#32-サンプルデバイスドライバi2c)
    - [3.2.1. 概要](#321-概要)
    - [3.2.2. デバイスドライバの使用方法](#322-デバイスドライバの使用方法)
- [4. プログラムの作成手順](#4-プログラムの作成手順)
  - [4.1. EWARMによるワークスペースおよびプロジェクト作成](#41-ewarmによるワークスペースおよびプロジェクト作成)
  - [4.2. μT-Kernel 3.0 BSP2と東芝社製BSPの組込み](#42-μt-kernel-30-bsp2と東芝社製bspの組込み)
    - [4.2.1. μT-Kernel 3.0 BSP2の組み込み](#421-μt-kernel-30-bsp2の組み込み)
      - [4.2.1.1. ソースコードの組込み](#4211-ソースコードの組込み)
    - [4.2.2. 東芝社製BSPの組み込み](#422-東芝社製bspの組み込み)
      - [4.2.2.1. ソースコードの組込み](#4221-ソースコードの組込み)
    - [4.2.3. EWARMに反映](#423-ewarmに反映)
    - [4.2.4. ビルド設定の追加](#424-ビルド設定の追加)
    - [4.2.5. OS起動処理の呼び出し](#425-os起動処理の呼び出し)
  - [4.3. ユーザプログラム](#43-ユーザプログラム)
    - [4.3.1. ユーザプログラムの作成](#431-ユーザプログラムの作成)
    - [4.3.2. usermain関数](#432-usermain関数)
    - [4.3.3. プログラム例](#433-プログラム例)
  - [4.4. ビルドおよび実行](#44-ビルドおよび実行)
    - [4.4.1. プログラムのビルド](#441-プログラムのビルド)
    - [4.4.2. デバッグの設定](#442-デバッグの設定)
    - [4.4.3. プログラムの実行](#443-プログラムの実行)
- [5. 変更履歴](#5-変更履歴)


# 1. はじめに
本書はμT-Kernel 3.0 BSP2の使用法について説明します。  
μT-Kenrel 3.0 BSP2は、マイコン・メーカなどから提供されるマイコンの開発環境、ツールを使用し、また同様に提供されるファームウェアなどを活用してリアルタイムOS μT-Kernel 3.0を使用するためのBSP(Board Support Package)です。  

本書では東芝デバイス&ストレージ株式会社のマイコンTMPM4Lを搭載したマイコンボードのμT-Kernel 3.0 BSP2について説明します。

## 1.1. 対象マイコンボード
μT-Kenrel 3.0 BSP2は以下の東芝のマイコンボードに対応しています。

| マイコンボード | マイコン     | CPUコア       | 備考                            |
| -------------- | ------------ | ------------- | ------------------------------- |
| SBK-M4L4       | TMPM4L4FYAUG | Arm Cortex-M4 | 東芝デバイス&ストレージ株式会社 |

## 1.2. 開発環境
開発環境は、IAR Sytems社の統合開発環境EWARM（IAR Embedded Workbench for ARM）を使用します。  
また、ファームウェアとして、東芝社製BSPの各種ソフトウェアを使用します。  

詳しくは以下のWebサイトをご覧ください。

統合開発環境EWARM（IAR Embedded Workbench for ARM）
https://www.iar.com/ja/

## 1.3. ソフトウェアの構成
μT-Kernel 3.0 BSP2は、リアルタイムOS μT-Kernel 3.0と、対象マイコンボード用の依存部プログラムおよびサンプルのデバイスドライバから構成されます。  
μT-Kernel 3.0は以下のバージョンを使用しています。  

- μT-Kernel 3.0 (v3.00.07)

μT-Kernel 3.0 BSP2のファイル構成を以下に示します。

- mtk3_bsp2  ルートディレクトリ
  - config  コンフィギュレーション定義ファイル
  - include インクルードファイル
  - mtkernel  OS本体のソースコード
  - sysdepend マイコン依存部のソースコード
    - toshiba_bsp  東芝マイコンおよび東芝社製BSP用のソースコード
      - CPU CPU(ARMコア)依存部のソースコード
      - lib ライブラリのハードウェア依存部
      - device　サンプル・デバイスドライバ

mtkernelディレクトリは、トロンフォーラムから公開されているμT-Kernel 3.0のGitサブモジュールです。なお、mtkernelディレクトリ下のソースコードは、ハードウェアに依存しない共通部のコードのみを使用しています。
マイコンボードやファームウェアに依存するソースコードはsysdependディレクトリにあります。  
deviceディレクトリには東芝社製BSPを利用したデバイスドライバのサンプル・プログラムがあります。I2CおよびA/DCの基本的な機能が使用可能です。  

# 2. BSP固有の機能について
## 2.1. デバッグ用シリアル出力
デバッグ用シリアル出力が使用できます。
この機能はコンフィギュレーションファイル(config/config.h)の以下を変更しビルドすることにより、有効無効を切り替えられます。

```c
/*---------------------------------------------------------------------- */
/* Use T-Monitor Compatible API Library  & Message to terminal.
 *  1: Valid  0: Invalid
 */
#define	USE_TMONITOR		(1)	/* T-Monitor API */
```

デバッグ用シリアル出力はT-Monitor互換API tm_printfが使用できます。tm_printfはC言語の標準関数printfとほぼ同等の機能を持ちますが、浮動小数点は使用できません。  
シリアル出力は以下の設定となります。  

| 項目       | 値        |
| ---------- | --------- |
| 速度       | 115200bps |
| データ長   | 8bit      |
| パリティ   | なし      |
| Stop Bit   | 1bit      |
| フロー制御 | 無し      |

シリアル信号はボードのmicro USB Type-Bコネクタに接続されます。具体的には以下のマイコンのペリフェラルを使用しています。  

| マイコンボード | ペリフェラル |
| -------------- | ------------ |
| SBK-M4L4       | UART0        |

## 2.2. 標準ヘッダファイルの使用
C言語の標準ヘッダファイルの使用が可能です。また、μT-Kernel 3.0のプログラムでも、<stddef.h>および<stdint.h>を使用しています。  
コンフィギュレーションファイル(config/config.h)の以下を変更することにより、標準ヘッダファイルは使用・不使用が指定できます。不使用とした場合、標準ヘッダファイルをユーザのプログラムからインクルードした際にエラーが発生することがあります。

```C
/*---------------------------------------------------------------------- */
/*
 *	Use Standard C include file
 */
#define USE_STDINC_STDDEF	(1)	/* Use <stddef.h> */
#define USE_STDINC_STDINT	(1) /* Use <stdint.h> */
```

## 2.3. FPU対応
本BSPはマイコン内蔵のFPUに対応しています。タスクのコンテキスト情報にFPUのレジスタなどの情報を含めます。  
コンフィギュレーションファイル(config/config.h)の以下を変更することにより、FPUの使用・未使用が指定できます。

```C
/*---------------------------------------------------------------------- */
/* Use Co-Processor.
 *  1: Valid  0: Invalid
 */
#define	USE_FPU			(1)	/* Use FPU */
```

FPUを使用するタスクは、タスク属性にTA_FPUを指定する必要があります。ただし、本マイコンの開発環境では、プログラムの一部に対してFPUの使用・不使用を指定することはできません。よってFPUを使用する場合は、すべてのタスクの属性にTA_FPUを指定することを推奨します。  
なお、コンフィギュレーションファイル(config/config.h)の以下を1に設定することにより、タスク属性の値いに関係なしにすべてのタスクをFPU対応とすることができます。  

```C
#define	ALWAYS_FPU_ATR		(1)	/* Always set the TA_FPU attribute on all tasks */
```

# 3. デバイスドライバ
## 3.1. サンプル・デバイスドライバ(A/DC)
### 3.1.1. 概要
A/DCデバイスドライバは、マイコン内蔵のA/Dコンバータを制御することができます。  
本BSPでは以下のデバイスに対応したA/DCデバイスドライバがあります。  

| デバイス名 | BSPのデバイス名 | 説明                         |
| ---------- | --------------- | ---------------------------- |
| ADC        | hadc            | Analog-to-digital converters |

デバイスドライバは内部の処理で東芝社製BSPを利用します。本デバイスドライバは東芝社製BSPをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にA/DCデバイスドライバのソースコードがあります。  
 
```mtk3_bsp2/sysdepend/toshiba_bsp/device/hal_adc```  

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/toshiba_bsp/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_ADC		1	// A/D conversion device
```

このデバイスドライバは内部で東芝社製BSPを呼び出します。東芝社製BSPとデバイスドライバの関連付けは次項で説明します。  

### 3.1.2. デバイスドライバの使用方法
(1) 東芝社製BSPの設定
A/DCデバイスドライバから使用するA/Dコンバータの設定を東芝社製BSPで行います。
東芝社製BSPは`bsp_adc_initialize`を使用することでA/Dコンバータの端子を設定します。

(参考) 各ボードのArduino互換インタフェースのアナログ入力(A0～A5)と、マイコンのA/Dコンバータの入力の対応は以下の通りです。

| Arduinoアナログ入力 | SBK-M4L4 |
| ------------------- | -------- |
| A0                  | ADC0_0   |
| A1                  | ADC0_1   |
| A2                  | ADC0_2   |
| A3                  | ADC0_3   |
| A4                  | ADC0_4   |
| A5                  | ADC0_5   |

(2) デバイスドライバの初期化  
A/DCデバイスドライバを使用するにあたり、最初に`dev_init_hal_adc`関数で初期化を行います。これにより、HALが関連付けられたA/DCデバイスドライバが生成されます。本関数は以下のように定義されます。  

```C
ER dev_init_hal_adc(
    UW unit,        // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
    bsp_adc_t *hadc         // ADCハンドラ構造体
);
```

パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータhadcは、東芝社製BSPで生成されている情報です。  
初期化に成功するとデバイス名`hadc*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hadca`、ユニット番号1のデバイス名は`hadcb`となります。  

μT-Kernel 3.0 BSP2の起動処理の`knl_start_device`関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/toshiba_bsp/devinit.c`

以下にknl_start_device関数の内容を示します。ここではボードごとにArduino互換インタフェースのアナログ入力で使用されるデバイスドライバを初期化しています。東芝社製BSPの`hadc1`,`hadc2`,`hadc3`が関連付けられたμT-Kernel 3.0のデバイスドライバが生成されます。  
実際に使用するA/Dコンバータに応じて変更してください。  

※SBK-M4L4の設定
SBK-M4L4では、ユニットごとに以下の設定になります。

| デバイス名 | A/Dチャンネル名 |
| ---------- | --------------- |
| hadca      | チャンネル4     |
| hadcb      | チャンネル4     |
| hadcc      | チャンネル17    |

```C
EXPORT ER knl_start_device( void )
{
  ER	err	= E_OK;

// 以下省略

#if DEVCNF_USE_HAL_ADC
	bsp_adc_t	hadc1;
        
  bsp_adc_initialize(&hadc1);
  
  NVIC_EnableIRQ(INTADASGL_IRQn);
        
	err = dev_init_hal_adc(DEV_HAL_ADC1, &hadc1);
	if(err < E_OK) return err;
#endif	/* DEVCNF_USE_HAL_ADC */

	return err;
}
```

(3) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、デバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後は同期リードAPI tk_srea_devによりデータを取得することができます。パラメータのデータ開始位置にA/DCのチャンネルを指定します。  
本デバイスドライバでは、一度のアクセスで一つのチャンネルから1データのみを取得できます。  

以下にA/DCデバイスドライバを使用したサンプル・プログラムを示します。  
このプログラムは500ms間隔でA/DCのチャンネル4からデータを取得し、その値をデバッグ用シリアル出力に送信するタスクの実行関数です。チャンネルの番号は実際に使用するA/Dコンバータに合わせてください。  

```C
LOCAL void task_1(INT stacd, void *exinf)
{
  UW	adc_val1;
	ID	dd;     // デバイスディスクリプタ
	ER	err;    // エラーコード

	dd = tk_opn_dev((UB*)"hadca", TD_UPDATE);    // デバイスのオープン
	while(1) {
		err = tk_srea_dev(dd, 0, &adc_val1, 4, NULL);   // A/DC チャンネル4からデータを取得
		tm_printf((UB*)"A/DC A0 =%06d\n", adc_val1);    // デバッグ出力
		tk_dly_tsk(500);       // 500ms待ち
}
```

## 3.2. サンプル・デバイスドライバ(I2C)
### 3.2.1. 概要
I2Cデバイスドライバは、マイコン内蔵のI2C通信デバイスを制御することができます。 
本BSPでは以下のデバイスに対応したI2Cデバイスドライバがあります。 

| デバイス名 | BSPのデバイス名 | 説明                                     |
| ---------- | --------------- | ---------------------------------------- |
| I2C        | hiic            | Inter-integrated circuit (I2C) interface |

デバイスドライバは内部の処理で東芝社製BSPを利用しています。このデバイスドライバは東芝社製BSPをμT-Kernel 3.0 で使用する方法を示すサンプルプログラムであり、デバイスの基本的な機能のみに対応しています。  
以下にI2Cデバイスドライバのソースコードがあります。

```mtk3_bsp2/sysdepend/toshiba_bsp/device/hal_i2c```

このデバイスドライバはBSPコンフィギュレーションファイル (config/config_bsp/toshiba_bsp/config_bsp.h) の以下を変更しビルドすることにより、使用・不使用を切り替えられます。

```C
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */
#define DEVCNF_USE_HAL_IIC		1	// I2C communication device
```

I2Cデバイスドライバは東芝社製BSPを使用していますので、東芝社製BSPのI2Cを使用可能にしてください。東芝社製BSPと本デバイスドライバの関連付けは次項で説明します。  

### 3.2.2. デバイスドライバの使用方法
(1) 東芝社製BSPの設定
I2Cデバイスドライバから使用するI2Cの設定を東芝社製BSPで行います。
東芝社製BSPは`bsp_ei2c_init`を使用することでA/Dコンバータの端子を設定します。

(参考) 各ボードのArduino互換インタフェースのI2C信号と、マイコンのI2C端子の対応は以下の通りです。  

| ボードのI2C信号 | SBK-M4L4     |
| --------------- | ------------ |
| Arduino I2C SCL | EI2C1SCL/PA5 |
| Arduino I2C SDA | EI2C1SDA/PA6 |

(2) デバイスドライバの初期化
I2Cデバイスドライバを使用するにあたり、最初に`dev_init_hal_i2c`関数で初期化を行います。これにより、指定したHALが関連付けられたI2Cデバイスドライバが生成されます。本関数は以下のように定義されます。   

```C
ER dev_init_hal_i2c(
    UW unit,                // デバイスのユニット番号(0～DEV_HAL_ADC_UNITNM)
    bsp_ei2c_t *hi2c // I2Cハンドル構造体
);
```
パラメータunitは0から順番に指定します。数字を飛ばすことはできません。  
パラメータhai2cは、東芝社製BSPで生成されている情報です。  
初期化に成功するとデバイス名`hiic*`のデバイスドライバが生成されます。デバイス名の`*`には`a`から順番に英文字が与えられます。ユニット番号0のデバイス名は`hiica`、ユニット番号1のデバイス名は`hiicb`、となります。

μT-Kernel 3.0 BSP2の起動処理のknl_start_device関数にてデバイスドライバの初期化を行っています。knl_start_device関数は以下のファイルに記述されています。  

`mtk3_bsp2/sysdepend/toshiba_bsp/devinit.c`

以下にknl_start_device関数の内容を示します。ここは実際のデバイスドライバの使用に応じて変更してください。

```C
EXPORT ER knl_start_device( void )
{
  ER	err	= E_OK;

#if DEVCNF_USE_HAL_IIC
  bsp_ei2c_t      bsp_ei2c;
  TXZ_Result      result;
  
  bsp_ei2c.ei2c.init.clock.sck = 100000;
  bsp_ei2c.ei2c.init.cr0.nfsel = EI2CxCR0_NFSEL_DIG;
  bsp_ei2c.ei2c.init.cr0.dnf = EI2CxCR0_DNF_3PRSCK;
  
  result = bsp_ei2c_init(&bsp_ei2c);
  
  NVIC_EnableIRQ(INTI2C1_IRQn);
  bsp_ei2c.ei2c.p_instance->AIE = EI2CxIE_INTSPE;

	err = dev_init_hal_i2c(DEV_HAL_I2C1, &bsp_ei2c);
	if(err < E_OK) return err;
#endif
        // 以下省略
}
```

(3) デバイスドライバの操作  
μT-Kernel 3.0のデバイス管理APIにより、デバイスドライバを操作できます。APIの詳細はμT-Kernel 3.0仕様書を参照してください。    
本デバイスドライバは、I2Cのコントローラ(マスター)モードにのみ対応します。  
最初にオープンAPI tk_opn_devにて対象とするデバイス名を指定しデバイスをオープンします。  
オープン後は同期リードAPI tk_srea_devによりデータの受信、同期ライトAPI tk_swri_devによりデータの送信を行うことができます。パラメータのデータ開始位置に対象のターゲット(スレーブ)アドレスを指定します。  

(4) デバイスのレジスタアクセス  
I2Cで接続されたターゲットデバイス内のレジスタをアクセスするための以下の関数が用意されています。これは比較的によく使用されるデバイス内のレジスタアクセス手順に対応しています。ただし、すべてのデバイスに使用できるわけではありませんので注意してください。

レジスタリード関数は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の受信を行います。  

```C
/* レジスタリード関数 */
ER i2c_read_reg(
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB *data  // リードしたデータ(1byte)
);

```
レジスタライト関数は、ターゲットデバイスに対して、レジスタアドレスの値(1byte)を送信し、続いてデータ(1byte)の送信を行います。  

```C
/* レジスタライト関数 */
ER i2c_write_reg(
    ID dd,    // デバイスディスクリプタ
    UW sadr,  // ターゲットアドレス
    UW radr,  // レジスタアドレス (下位8bitのみ有効)
    UB data  // ライトするデータ(1byte)
);
```

# 4. プログラムの作成手順
EWARMでプログラムのプロジェクトを作成し、μT-Kenrel 3.0 BSP2を組み込んでビルド、実行までの手順を説明します。

## 4.1. EWARMによるワークスペースおよびプロジェクト作成
以下の手順で対象マイコンボードのワークスペースを作成します。これはEWARMの標準の手順です。

(1) メニュー[ファイル]→[新規ワークスペース]で新規のワークスペースを作成します。
(2) メニュー[ファイル]→[名前を付けてワークスペースを保存]で任意の場所にワークスペースを作成し保存します。

以下の手順で対象マイコンボードのプロジェクトを作成します。これはEWARMの標準の手順です。

(1) メニュー[プロジェクト]→[新規プロジェクトの作成]を選択します。  
(2) [Arm]→[C]→[main]を選択し、任意の場所にプロジェクトを作成し保存します。

## 4.2. μT-Kernel 3.0 BSP2と東芝社製BSPの組込み
### 4.2.1. μT-Kernel 3.0 BSP2の組み込み
#### 4.2.1.1. ソースコードの組込み
作成したプロジェクトにμT-Kernel 3.0 BSP2のソースコードを組込みます。  
以下のGitHubのリポジトリからμT-Kernel 3.0 BSP2から入手し、プロジェクトのディレクトリ中にμT-Kernel 3.0 BSP2のソースコードのディレクトリmtk3_bsp2を置きます。

`https://github.com/tron-forum/mtk3_bsp2.git`

gitのコマンドを使用する場合は、プロジェクトのディレクトリをカレントディレクトリとし、以下のコマンドを実行します。  

`git clone --recursive  https://github.com/tron-forum/mtk3_bsp2.git`

μT-Kernel 3.0 BSP2はμT-Kernel 3.0のリポジトリをgitのサブモジュールとして内包していますので、全ソースコードを取得するには--recursiveが必要となります。  

### 4.2.2. 東芝社製BSPの組み込み
#### 4.2.2.1. ソースコードの組込み
Web上から東芝社製のサンプルプログラムを入手します。

入手したら`TMPM4LxA`のフォルダを使用しますので、手動で任意の場所に追加します。

### 4.2.3. EWARMに反映
取り込んだμT-Kernel 3.0 BSP2と東芝社製BSPは、EWPtoolを使用してEWARMに反映させることができます。
EWPtoolのダウンロードとインストール方法および使用方法は以下のGitHubから参照可能です。
`https://github.com/IARSystems/project-migration-tools`


### 4.2.4. ビルド設定の追加
プロジェクトのプロパティに、μT-Kernel 3.0 BSP2のスースコードをビルドするための設定を追加します。
プロジェクトを選択し、右クリックメニュー→[オプション]で以下を設定します。

(1) [C/C++ コンパイラ]→[プリプロセッサ]→[シンボル定義]
対象マイコンボードのターゲット名を設定します。

| マイコンボード | ターゲット名            |
| -------------- | ----------------------- |
| SBK-M4L4       | \_TOSHIBA_BSP_SBK_M4L4_ |

(2) [C/C++ コンパイラ]→[プリプロセッサ]→[追加インクルードディレクトリ]
以下を設定します。  
プロジェクトのディレクトリの直下にμT-Kernel 3.0 BSP2のディレクトリmtk3_bsp2と東芝社製BSPのTMPM4LxAがある前提です。   

```
$PROJ_DIR$\TMPM4LxA\Utility\inc
$PROJ_DIR$\TMPM4LxA\BSP\SBK_M4L4\inc
$PROJ_DIR$\TMPM4LxA\BSP\SBK_M4L4\src
$PROJ_DIR$\TMPM4LxA\MCU\Driver\TSB_driver\inc
$PROJ_DIR$\TMPM4LxA\MCU\CMSIS
$PROJ_DIR$\TMPM4LxA\MCU\CMSIS_M4L
$PROJ_DIR$\TMPM4LxA\MCU\Driver\TSB_driver\src
$PROJ_DIR$\TMPM4LxA\MCU\Driver\TSB_driver\config\M4L
$PROJ_DIR$\mtk3_bsp2
$PROJ_DIR$\mtk3_bsp2\config
$PROJ_DIR$\mtk3_bsp2\include
$PROJ_DIR$\mtk3_bsp2\mtkernel\kernel\knlinc
```

(3) [アセンブラ]→[プリプロセッサ]→[シンボル定義]
(1)と同じ設定を行います。  

(4) [アセンブラ]→[プリプロセッサ]→[追加インクルードディレクトリ]
以下を設定します。  
プロジェクトのディレクトリの直下にμT-Kernel 3.0 BSP2のディレクトリmtk3_bsp2と東芝社製BSPのTMPM4LxAがある前提です。   

```
$PROJ_DIR$\mtk3_bsp2
$PROJ_DIR$\mtk3_bsp2\config
$PROJ_DIR$\mtk3_bsp2\include
$PROJ_DIR$\mtk3_bsp2\mtkernel\kernel\knlinc
```

### 4.2.5. OS起動処理の呼び出し
生成されたプロジェクトは、ハードウェアの初期化処理などのあと、main関数を実行します。  
main関数からμT-Kernel 3.0の起動処理knl_start_mtkernel関数を実行するようにコードを追加します。
main関数は以下のファイルの記述されています。

`<プロジェクトのディレクトリ>/main.c`

```C
#include "bsp.h"

int main()
{
  /*----------------------*/
  /* System initialize    */
  /*----------------------*/
  SystemInit();
  
  /*----------------------*/
  /* BSP initialize       */
  /*----------------------*/
  bsp_initialize();
    
  void knl_start_mtkernel(void);
  knl_start_mtkernel();
  return 0;
}
```
## 4.3. ユーザプログラム
### 4.3.1. ユーザプログラムの作成
μT-Kernel 3.0で実行するユーザプログラムを作成します。  
ユーザプログラムを作成するディレクトリは名前も場所も任意です。たとえば、プロジェクトのディレクトリ・ツリーの最上位にapplicationなどといった名称のディレクトリを作成します。

### 4.3.2. usermain関数
μT-Kernel 3.0は起動するとユーザプログラムのusermain関数を実行します。よってusermain関数を定義してください。  
usermain関数は以下の形式とします。

```C
INT usermain(void);
```

usermain関数は、μT-Kernel 3.0が最初に生成、実行したタスク（初期タスク）から呼ばれます。  
また、usermain関数が終了するとμT-Kernel 3.0はシャットダウンします。よって、通常はusermain関数は終了させずに、μT-Kernel 3.0のタスク休止API tk_slp_tskなどを使用して待ち状態とします。
初期タスクは優先度が高く設定されてますので、usermain関数の実行中は他のタスクは実行されません。

なお、ユーザプログラムでusermain関数を定義しなかった場合は、以下のファイルに記述されたデフォルトのusermain関数が実行されます。
このusermain関数は何もせずに終了します。よってμT-Kernel 3.0はすぐにシャットダウンをします。

`mtk3_bsp2/mtkernel/kernel/usermain/usermain.c`

デフォルトのusermain関数はweak属性が指定されてますので、ユーザプログラムにusermain関数が存在する場合は無効となります。ファイルを変更する必要はありません。

### 4.3.3. プログラム例
以下にユーザプログラムの例を示します。このプログラムは二つのタスクが実行されます。どちらのタスクも一定間隔でデバッグ用シリアル出力に文字列を表示します。

```C
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

LOCAL void task_1(INT stacd, void *exinf);	// task execution function
LOCAL ID	tskid_1;			// Task ID number
LOCAL T_CTSK ctsk_1 = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= task_1,
	.tskatr		= TA_HLNG | TA_RNG3,
};

LOCAL void task_2(INT stacd, void *exinf);	// task execution function
LOCAL ID	tskid_2;			// Task ID number
LOCAL T_CTSK ctsk_2 = {				// Task creation information
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= task_2,
	.tskatr		= TA_HLNG | TA_RNG3,
};

LOCAL void task_1(INT stacd, void *exinf)
{
	while(1) {
		tm_printf((UB*)"task 1\n");
		tk_dly_tsk(500);
	}
}

LOCAL void task_2(INT stacd, void *exinf)
{
	while(1) {
		tm_printf((UB*)"task 2\n");
		tk_dly_tsk(700);
	}
}

/* usermain関数 */
EXPORT INT usermain(void)
{
	tm_putstring((UB*)"Start User-main program.\n");

	/* Create & Start Tasks */
	tskid_1 = tk_cre_tsk(&ctsk_1);
	tk_sta_tsk(tskid_1, 0);

	tskid_2 = tk_cre_tsk(&ctsk_2);
	tk_sta_tsk(tskid_2, 0);
	
	tk_slp_tsk(TMO_FEVR);

	return 0;
}
```

## 4.4. ビルドおよび実行
### 4.4.1. プログラムのビルド
プロジェクトを選択し、メニュー[プロジェクト]→[すべてを再ビルド]または右クリックメニューにより、プロジェクトをビルドします。

### 4.4.2. デバッグの設定
使用するデバッガに応じてデバッグの設定を行います。
SBK-M4L4ボードではデバッガはボード上に搭載されています。

### 4.4.3. プログラムの実行
マイコンボードをPCに接続し、[ダウンロードしてデバッグ]ボタンをクリックすると、実行プログラムがマイコンボードに転送され、デバッグ実行が開始します。
実行プログラムはマイコンボードのフラッシュメモリーに書き込まれますので、デバッガを外した状態でマイコンボードの電源を入れれば、そのまま実行されます。
なお、マイコンボードによってはデバッグ実行を行うために、ボード上のスイッチなどを設定する必要があるものがあります。マイコンボードのマニュアルなどをご覧ください。  


# 5. 変更履歴

| 版数    | 日付       | 内容 |
| ------- | ---------- | ---- |
| 1.00.00 | 2025.11.26 | 初版 |
// インクルードガード
#ifndef ECHO_CANCEL_H
#define ECHO_CANCEL_H

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <string.h>
#include <math.h>

#define FRAME_SIZE 256
#define FILTER_LENGTH 128
#define LEARNING_RATE 0.01

// LMSフィルタの初期化
void init_filter(complex double *filter, int length) {
    for (int i = 0; i < length; i++) {
        filter[i] = 0.0 + 0.0 * I;
    }
}

// エコー推定
void estimate_echo(complex double *filter, complex double *input, complex double *output, int length) {
    for (int i = 0; i < length; i++) {
        output[i] = 0.0 + 0.0 * I;
        for (int j = 0; j < FILTER_LENGTH; j++) {
            if (i - j >= 0) {
                output[i] += filter[j] * input[i - j];
            }
        }
    }
}

// LMSフィルタの更新
void update_filter(complex double *filter, complex double *input, complex double *error, int length) {
    for (int i = 0; i < FILTER_LENGTH; i++) {
        complex double gradient = 0.0 + 0.0 * I;
        for (int j = 0; j < length; j++) {
            if (j - i >= 0) {
                gradient += error[j] * conj(input[j - i]);
            }
        }
        filter[i] += LEARNING_RATE * gradient;
    }
}

/* 実装例
#define FRAME_SIZE 256
#define FILTER_LENGTH 128
#define LEARNING_RATE 0.01

    // フィルタ係数の初期化
    complex double filter[FILTER_LENGTH];
    init_filter(filter, FILTER_LENGTH);

    // マイク入力信号とスピーカー出力信号のサンプル
    complex double mic_input[FRAME_SIZE];
    complex double speaker_output[FRAME_SIZE];
    complex double estimated_echo[FRAME_SIZE];
    complex double error_signal[FRAME_SIZE];

    // マイク入力信号とスピーカー出力信号の読み込み（例: ファイルから）
    FILE *mic_fp = fopen("mic_input.raw", "rb");
    FILE *spk_fp = fopen("speaker_output.raw", "rb");
    if (!mic_fp || !spk_fp) {
        perror("Error opening file");
        return -1;
    }

    while (fread(mic_input, sizeof(complex double), FRAME_SIZE, mic_fp) == FRAME_SIZE &&
           fread(speaker_output, sizeof(complex double), FRAME_SIZE, spk_fp) == FRAME_SIZE) {
        
        // エコー推定
        estimate_echo(filter, speaker_output, estimated_echo, FRAME_SIZE);

        // エコーキャンセル
        for (int i = 0; i < FRAME_SIZE; i++) {
            error_signal[i] = mic_input[i] - estimated_echo[i];
        }

        // フィルタの更新
        update_filter(filter, speaker_output, error_signal, FRAME_SIZE);
    }

    fclose(mic_fp);
    fclose(spk_fp);
*/

#endif
% atividade 4

% data =  vetor de samples
% sr = sample rate
% bps = bits per sample
[data, sr, bps] = wavread("../audio/yamaha.wav");

figure; plot(data);

fft_data = fft(data);
fft_real_data = real(fft_data);

figure; plot(fft_real_data);

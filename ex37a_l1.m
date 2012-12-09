% exercicio 37
close all; clear all; clc
% a)

%  sinetone (freq, rate, sec, ampl)
%    freq = frequency
%    sec  = length (seconds)
%    rate = sampling rate
%    ampl = amplitude
%  freq and ampl may be vectors of common size.
%  defaults are rate = 8000, sec = 1 and ampl = 64.

freq = 435;
rate = 8000;
sec  = 5;
ampl = 16;

%data = sinetone(freq, rate, sec, ampl);
%wavwrite(data, "ex37a_l1.wav");

bps = 16;       % bits per sample
sps = 8000;     % sample rate [samples/s]
freq = 435;       % frequency of the tone [Hz]
nsecs = 5;      % number of seconds of the audio file

nsamples = sps*nsecs;

time = linspace(0, nsecs, nsamples);
wave = sin(time*2*pi*freq)';

wavwrite(wave, sps, bps, 'ex37a_l1.wav');
figure, plot(wave);

harm = zeros(size(wave));
for i = 1:5
	harm = harm + (sin(time*i*2*pi*freq)' / i);
end

wavwrite(harm, "ex37b_l1.wav");
figure, plot(harm);

harm = zeros(size(wave));
for i = 1:10
	harm = harm + (sin(time*i*2*pi*freq)' / i);
end

wavwrite(harm, "ex37c_l1.wav");
figure, plot(harm);

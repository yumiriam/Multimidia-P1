bps = 16;       % bits per sample
sps = 8000;     % sample rate [samples/s]
freq = 435;       % frequency of the tone [Hz]
nsecs = 5;      % number of seconds of the audio file

nsamples = sps*nsecs;

time = linspace(0, nsecs, nsamples);
wave = sin(time*2*pi*freq)';

% create the amplitude window (creates the fade in and fade out when multiplied with the sine wave)
a = b = floor(.3*nsamples);
window = [linspace(0,1,a), ones(1,nsamples-a-b), linspace(1,0,b)]';


wavwrite(wave.*window, sps, bps, 'audio.wav')

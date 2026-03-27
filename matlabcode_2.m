%Assignemnt 4_2
clear 
clc
close all

F_s = 200;      
SampleTime = 4;
SampleNumber = SampleTime*F_s;

%% getting data from Arduino
a = strings(SampleNumber,1);

% COM9 was used for Arduino
ss = serialport("COM9", 57600);
configureTerminator(ss,"CR/LF");
Time = linspace(0,SampleTime,SampleNumber);
i=1;

% Using the SampleNumber with a while loop we take data from Arduino
while i<=SampleNumber
 a(i,1) = readline(ss);
 if  i==SampleNumber
    break;
 end
i = i+1;
end

% putting the data taken from Arduino into Signal variable
Signal = double(a(:,1));
clear ss;

%% calculating filter transfer func. 

R = 220;  % Resistor 220Ω is used in the LOW_Pass filter
C = 10^-4;   % Capacitor 100µF is used in The LOW_Pass filter
s = tf('s');
G = 1/(R*C*s+1);    % filter transfer func. 

f1 = 4;
f2 = 61;
t = 0:0.001:SampleTime;
u = 1.25*(sin(2*pi*f1*t) + sin(2*pi*f2*t)) + 2.5;

%% plotting the expected signal (y) and data points taken from Arduino
y = lsim(G, u, t);
figure(1);
plot(t, y);
hold on
d = plot(Time,Signal,'.black');
d(1).MarkerSize =20;
ylim([0,5]);
xlim([0,2]);
hold off
title('f cutoff = 8 Hz , f1 = 4 Hz , f1 = 61 Hz , f sampling = 200 Hz');
xlabel('time(s)');
ylabel('amplitude(v)');
legend('expected signal', 'points');
%% calculating frequency spectrum before applying hanning
F = F_s/(SampleNumber)*[0:SampleNumber-1];
Mag = abs(fft(Signal));
Mag = Mag/(SampleNumber/2);
Mag(1,1) = Mag(1,1)/2;

% plotting the frequancy spectrum before applying hanning
figure(2);
stem (F(1:SampleNumber/2+1), Mag(1:SampleNumber/2+1));
xlabel('Frequency(Hz)');
ylabel('|X|');
xlim([0,100]);
title('spectrum of signal before applying hanning')

%% applying hanning and calculating frequency spectrum
hanning = hann(SampleNumber);
Offset = 2.5;
New_Signal = Signal-Offset;
New_Signal = New_Signal.*hanning;
Mag = abs(fft(New_Signal));
Mag = Mag/(SampleNumber/2);
Mag(1,1) = Mag(1,1)/2;
Mag = Mag*2;
Mag(1,1) = Offset;

%% plotting the frequancy spectrum after applying hanning
figure(3);
stem (F(1:SampleNumber/2+1), Mag(1:SampleNumber/2+1));
xlabel('Frequency(Hz)');
ylabel('|X|');
xlim([0,100]);
title('spectrum of signal after applying hanning')

%% calculating attenuation
frequencies = [4,61];
attenuation_theorical(1) = 20*log10(1/sqrt(1+(4/8)^2));
attenuation_experimental(1) = 20*log10(Mag(17)/1.25);
attenuation_theorical(2) = 20*log10(1/sqrt(1+(61/8)^2));
attenuation_experimental(2) = 20*log10(Mag(245)/1.25);
fprintf('at 4 Hz theorical and experimental attenuation is equal to %f db and %f db\n',attenuation_theorical(1),attenuation_experimental(1))
fprintf('at 61 Hz theorical and experimental attenuation is equal to %f db and %f db\n',attenuation_theorical(2),attenuation_experimental(2))
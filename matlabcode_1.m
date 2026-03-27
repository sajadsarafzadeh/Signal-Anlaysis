%assignment 4_1
clear
clc
close all
SineFrequancy = 23;
F_s = 120;
SampleTime = 1;
SampleNumber = SampleTime*F_s;

%% getting data from Arduino
ss = serialport("COM9", 57600);  % COM9 is used for Arduino
a = strings(SampleNumber,1);
configureTerminator(ss,"CR/LF");
Time = linspace(0,SampleTime,SampleNumber);
i=1;

% Using the SampleNumber we build a while where we take data from Arduino
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
%% calculating filter trasfer func.
R = 1591;  % Resistor 1591Ω is used in the LOW_Pass filter
C = 10^-6;  % Capacitor 1µF is used in The LOW_Pass filter
s = tf('s');
G = 1/(R*C*s+1);   %filter transfer function

% plotting amplitude of signal that we used
t = 0:0.001:SampleTime;
u = 2.5*sin(2*pi*SineFrequancy*t) + 2.5;

%% plotting the expected signal (y) and data points gotten from Arduino
y = lsim(G, u, t);
figure(1);
plot(t, y,'LineWidth',1);
hold on
d = plot(Time,Signal,'.black');
d(1).MarkerSize =15;
ylim([0,5]);
xlim([0,SampleTime]);
plot(Time,Signal,'red','LineWidth',2);
hold off
title('f cutoff = 100 Hz , f = 23 Hz, f sampling = 120 Hz');
xlabel('time(s)');
ylabel('amplitude(v)');
legend('expected signal', 'points', 'Arduino signal');

%% calculating and plotting frequency spectrum
Mag = abs(fft(Signal));
Mag = Mag/(SampleNumber/2);
Mag(1,1) = Mag(1,1)/2;

F = F_s/(SampleNumber)*[0:SampleNumber-1];
figure(2);
stem (F(1:SampleNumber/2+1), Mag(1:SampleNumber/2+1));
xlabel('Frequency(Hz)');
ylabel('|X|');
title('spectrum of signal, fsampling = 120 Hz ')
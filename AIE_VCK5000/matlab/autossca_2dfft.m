function [tSx,Sx,alphao,fo] = autossca_2dfft(x,fs,Np,N)
    %autossca_2dfft compute ssca through 2DFFT
    %   Detailed explanation goes here
    
%   correlation density function estimate of the signals X,
%   by using the Strip Spectral Correlation Algorithm (SSCA).
%
%   INPUTS:
%   X       - input column vector;
%   FS      - sampling rate;
%   Np      - N' band channelizer;
%   N       - input data length.
%   M       - sqrt of N
%
%   OUTPUTS:
%   SX      - spectral correlation density function estimate;
%   ALPHAO  - cyclic frequency; and
%   FO      - spectrum frequency.

M = sqrt(N);

if nargin ~= 4
    error('Wrong number of arguments.');
end
%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Input Channelization %%
%%%%%%%%%%%%%%%%%%%%%%%%%%
if length(x) < N+Np
    x(N)=0;
    disp('you will not get the desired resolution in cyclic frequency');
    dalpha=fs/N;
    disp(['cyclic frequency resolution=', num2str(dalpha)]);
elseif length(x) > N+Np
    x=x(1:N+Np);
end

X=zeros(Np,N);
for k=0:N-1
    X(:,k+1)=x(k+1:k+Np);
end

a = chebwin(Np);
tintm = complex(zeros(Np,N));
for i = 1:M
    temp = X(:,i:M:end);
    tXW=diag(a)*temp;
    tXF1=fft(tXW,[],1);
    tE = exp(-j*2*pi*[0:Np-1]*(i-1)/Np).';
    tXD = tXF1.*tE;
    txc = ones(Np,1)*x(Np/2+i:M:Np/2+N)';
    tXM = tXD.*txc;
    % tintm(:,(i-1)*M+1:i*M) = tXM;

    tXFFT_s1=fft(tXM,[],2);
    trotf = single(ones(Np,1)*exp(-j*2*pi*(i-1)*[1:M]/(M*M)));
    XFFT_s1 = trotf.*tXFFT_s1;
    tintm(:,i:M:N) = XFFT_s1;
end
tSx = complex(zeros(Np,N));
for i = 1:M
    temp = tintm(:,(i-1)*M+1:i*M);
    tXFFT_s2 = fft(temp,[],2);
    tSx(:,i:M:N) = tXFFT_s2;
end
%%%%%%%%%%%Finish the AIE section%%%%%%%%%%%%%

dataout=fftshift(tSx, 1);
dataout=fftshift(dataout, 2);
Ma=abs(dataout.'); %SCD
% M=abs(XF2./abs(XF2)); % coherence??SCF seems not right by CL
alphao=(-1:1/N:1-1/N); 
fo=(-.5:1/Np:.5); 
Sx=zeros(Np,2*N);

for k1=-N/2:N/2-1%1:N
    for k2=-Np/2:Np/2-1%1:Np

        alpha=(k1)/N+(k2)/Np;
        f=((k2)/Np-(k1)/N)/2;
        k=ceil(1+Np*(f+.5));
        l=1+N*(alpha+1);
        Sx(k,l)=Ma(k1+N/2+1,k2+Np/2+1);
    end
end
end
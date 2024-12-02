function [XF2.',Sx,alphao,fo]=autossca(x,fs,Np,N)
%   AUTOSSCA(X,FS,DF,DALPHA) computes the spectral auto-
%   correlation density function estimate of the signals X,
%   by using the Strip Spectral Correlation Algorithm (SSCA).
%
%   INPUTS:
%   X       - input column vector;
%   FS      - sampling rate;
%   Np      - N' band channelizer;
%   N       - input data length.
%
%   OUTPUTS:
%   SX      - spectral correlation density function estimate;
%   ALPHAO  - cyclic frequency; and
%   FO      - spectrum frequency.


if nargin ~= 4
    error('Wrong number of arguments.');
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Definition of Parameters %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

                                

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
% x(N+1:N+Np) = x(1:Np);
% x(N+1:N+Np) = zeros(Np);
X=zeros(Np,N);
for k=0:N-1
    X(:,k+1)=x(k+1:k+Np);
end

%%%%%%%%%%%%%%%
%% Windowing %%
%%%%%%%%%%%%%%%
% a=hamming(Np);
a = chebwin(Np);
XW=diag(a)*X;

%%%%%%%%%%%%%%%
%% First FFT %%
%%%%%%%%%%%%%%%
XF1=fft(XW,[],1);
% XF1=fftshift(XF1, 1)./Np;
XF1=fftshift(XF1, 1);


%%%%%%%%%%%%%%%%%%%%
%% Downconversion %%
%%%%%%%%%%%%%%%%%%%%
E=zeros(Np,N);

for k=-Np/2:Np/2-1
    for m=0:N-1
        E(k+Np/2+1,m+1)=exp(-j*2*pi*k*m/Np);
    end
end

XD=XF1.*E;

%%%%%%%%%%%%%%%%%%%%
%% Multiplication %%
%%%%%%%%%%%%%%%%%%%%
xc=ones(Np,1)*x(Np/2+1:Np/2+N)'; %conjugate(x) and transpose
XM=XD.*xc;
XM=XM.';

%%%%%%%%%%%%%%%%
%% Second FFT %%
%%%%%%%%%%%%%%%%
XF2=fft(XM,[],1);
XF2=fftshift(XF2,1);
% XF2=[XF2(:,Np/2+1:Np) XF2(:,1:Np/2)];
M=abs(XF2); %SCD
% M=abs(XF2./abs(XF2)); % coherence??SCF seems not right by CL
alphao=(-1:1/N:1-1/N); 
fo=(-.5:1/Np:.5); 
Sx=zeros(Np,2*N);

%%%%%%%%%%%%%%%%%%%
%% alpha profile %%
%%%%%%%%%%%%%%%%%%%
for k1=-N/2:N/2-1%1:N
    for k2=-Np/2:Np/2-1%1:Np
%         k = k2-Np/2-1;
%         q = k1-N/2-1;
%         freq = (k/Np-q/N)/2;
%         alpha = k/Np+q/N;
%         idxfreq = find(abs(fo-freq)==min(abs(fo-freq)));
%         idxalpha = find(abs(alphao-alpha)==min(abs(alphao-alpha)));
%         Sx(idxalpha,idxfreq)=M(k1,k2);
        alpha=(k1)/N+(k2)/Np;
        f=((k2)/Np-(k1)/N)/2;
        k=ceil(1+Np*(f+.5));
        l=1+N*(alpha+1);
        Sx(k,l)=M(k1+N/2+1,k2+Np/2+1);
    end
end
end

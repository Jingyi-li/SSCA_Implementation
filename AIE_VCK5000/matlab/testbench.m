%% test fft 16 groups of 64point FFT
% test_fft_16x64pt(1,0,0);

%-------------------------------------------------------
% Generate Input
%-------------------------------------------------------
N = 2^20;
Np = 64;
% a = single(randn(2, N));
a = load('chad_dsss_noisefree_10dB_data.dat');
tpwx_dat = complex(a(:,1), a(:,2));
tpwx_dat(N+1:N+Np) = complex(zeros(Np,1));

%-------------------------------------------------------

% [~,Sx_2dfftssca,~,~] = autossca_2dfft(tpwx_dat,1,Np,N);
% [~,Sx_ssca,~,~] = autossca(tpwx_dat,1,Np,N);

% figure
% hold on
% plot (max(Sx_2dfftssca,[],1));
% plot (max(Sx_ssca,[],1));

%%%%%%%%%%%%%%%%%%%%%%
% Check accuracy
%%%%%%%%%%%%%%%%%%%%%%
[Sx_2dfftssca,~,~,~] = autossca_2dfft(tpwx_dat,1,Np,N);

fid = fopen('../host/output.dat', 'r');
if fid == -1
    error('Could not open file');
end
Sx_aie = zeros(Np,N);
for i = 1:1024
    chunk = textscan(fid, "%f %f", 64*1024, 'Delimiter', '\t');
    chunkMatrix = complex(chunk{1}, chunk{2}); chunkMatrix = reshape(chunkMatrix,[],1024);
    Sx_aie(:,i:1024:end) = chunkMatrix;
end

disp("finish autossca_2dfft");


norm(Sx_ssca.' - Sx_ssca,1)/norm(Sx_ssca.',1);
function [dataout] = aie_model_ssca_fp(datain,win_id)
%AIE_MODEL_SSCA_FP Summary of this function goes here
%   Detailed explanation goes here
Np = 64;
temp = reshape(datain, 16, Np);
temp_win = temp.*chebwin(Np).';
temp_fft = fft(temp_win,[],2);
E=zeros(1,Np);
for k=-Np/2:Np/2-1
    for m=win_id
        E(m+1,k+Np/2+1)=exp(-j*2*pi*k*m/Np);
    end
end
temp_dc = temp_fft.* E;
xc = conj(temp(:,33));
temp_cdp = temp_dc .* xc;
idx = [0,2,4,6,1,3,5,7,8,10,12,14,9,11,13,15];
for i = 1:16    
    dataout(i,:) = temp_cdp(idx(i)+1,:);
end
end


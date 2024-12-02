%%%%%%%%%%%%%%%%%%%%%%%
% CDP Section
%%%%%%%%%%%%%%%%%%%%%%%
Np = 64;
N = 2^20;
%%%%%%%%%%%%%%%%%%%%%%%
% Print input test
%%%%%%%%%%%%%%%%%%%%%%%


data = temp(:,1:16).';
data = reshape(data,[],1);


for t=1:2
		
    % input in int32 format
    a = data(t:2:end);
    a = reshape(a.', 1, []); a = [real(a);imag(a)]; a = float2int(a);

    fid = fopen(sprintf('../aie/data/cdp_test_%02d%c.txt', i-1, 'a'+t-1), 'wt');
    fprintf(fid, '%d %d\n', a);
    fclose(fid);

end
%%%%%%%%%%%%%%%%%%%%%%%
% Print input test
%%%%%%%%%%%%%%%%%%%%%%%
data = tXM(:,1:16).';
data = reshape(data,[],1);

for t=1:2
		
    % input in int32 format
    a = data(t:2:end);
    a = reshape(a.', 1, []); a = [real(a);imag(a)]; a = float2int(a);

    fid = fopen(sprintf('../aie/data/cdp_test_output_%02d%c.txt', i-1, 'a'+t-1), 'wt');
    fprintf(fid, '%d %d\n', a);
    fclose(fid);

end

%%%%%%%%%%%%%%%%%%%%%%%
% Check CDP
%%%%%%%%%%%%%%%%%%%%%%%
fid = fopen('../host/output.dat', 'r');
if fid == -1
    error('Could not open file');
end
data = [];
output = zeros(1024,1);

for ic = 1:1024
    chunk = textscan(fid, "%f %f", 64, 'Delimiter', '\t');
    chunkMatrix = complex(chunk{1}, chunk{2}); 
    output(i) = sum(tXM(:,ic)-chunkMatrix);
end
max(output)
fclose(fid);


%%
%%%%%%%%%%%%%%%%%%%%%%%
% Check FFTs1
%%%%%%%%%%%%%%%%%%%%%%%
fid = fopen('../host/output_fft1.dat', 'r');
if fid == -1
    error('Could not open file');
end
data = [];

chunk = textscan(fid, "%f %f", 64*1024, 'Delimiter', '\t');
chunkMatrix = complex(chunk{1}, chunk{2}); chunkMatrix = reshape(chunkMatrix,[],1024);
output = XFFT_s1-chunkMatrix;
max(max(output))

fclose(fid);


%%
%%%%%%%%%%%%%%%%%%%%%%%
% Check FFTs1 compare as a whole
%%%%%%%%%%%%%%%%%%%%%%%
fid = fopen('../host/output.dat', 'r');
if fid == -1
    error('Could not open file');
end
data = [];
output = zeros(1024,1);

check_xffts1 = zeros(Np,N);
for ii = 1:10
    for ic = 1:1024
        chunk = textscan(fid, "%f %f", 64, 'Delimiter', '\t');
        check_xffts1(:,(ic-1)*1024+ii) = complex(chunk{1}, chunk{2}); 
    end
end
fclose(fid);

outputdiff = check_xffts1(:,1:10)-tintm(:,1:10);

%%
%%%%%%%%%%%%%%%%%%%%%%%
% Check FFTs2
%%%%%%%%%%%%%%%%%%%%%%%
fid = fopen('../host/output.dat', 'r');
if fid == -1
    error('Could not open file');
end
data = [];
output = zeros(1024,1);

check_xffts2 = zeros(Np,N);
for ii = 1:1024
    for ic = 1:1024
        chunk = textscan(fid, "%f %f", 64, 'Delimiter', '\t');
        check_xffts2(:,(ic-1)*1024+ii) = complex(chunk{1}, chunk{2}); 
    end
end
fclose(fid);

outputdiff = check_xffts2(:,1:10)-tSx(:,1:10);


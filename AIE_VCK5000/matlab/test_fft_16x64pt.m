%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

function test_fft_16x64pt(N_symb, InputMode, TODEBUG)

if ~exist('N_symb', 'var')
	N_symb = 4;
	rng(12345);
else
	rng shuffle;
end

N_Parallel = 1;
N_datablk = 1024;
N_window = 64;


%-------------------------------------------------------
% Generate Random Input
%-------------------------------------------------------
rng(12345);
a = single(randn(2, N_symb*N_datablk));
tpwx_dat = complex(a(1,:), a(2,:));
%-------------------------------------------------------

% %-------------------------------------------------------
% % Generate Input
% %-------------------------------------------------------
% a = single([1:64]/64);
% b = zeros(1,N_datablk);
% % for i = 1:16
% %     b(i:(N_datablk/N_window):end) = a;
% % end
% b((3+1):(N_datablk/N_window):end) = a;
% a = [b;b];
% tpwx_dat = complex(a(1,:), a(2,:));
% %-------------------------------------------------------

% %-------------------------------------------------------
% % Generate Input after window
% %-------------------------------------------------------
% a = single([1:64].'/64.*chebwin(64));
% b = zeros(1,N_datablk);
% b((13+1):(N_datablk/N_window):end) = a;
% a = [b;b];
% tpwx_dat = complex(a(1,:), a(2,:));
% %-------------------------------------------------------




%-------------------------------
% Allocate memory for output
%-------------------------------
y     = zeros(1, N_datablk*N_symb);
%--------------------------------

%-----------------------------------
% Save test vectors
%-----------------------------------
if ~exist('../aie/data', 'dir'), mkdir('../aie/data'); end


% (1) FFT input and output data
idx = reshape(1:1024, 8, []);
idx = reshape([idx(1:4, :), idx(5:8, :)], 1, []);

for k=1:N_symb

	if(k==1)
		opmod = 'wt';
	else
		opmod = 'at';
	end


    xx = reshape(tpwx_dat((k-1)*N_datablk + (1:N_datablk)), 1024, []);
	
	for phase=1:N_Parallel
	
		for t=1:2
		
            % input in int32 format
            a = xx(t:2:end);
		    a = reshape(a.', 1, []); a = [real(a);imag(a)]; a = float2int(a);

		    fid = fopen(sprintf('../aie/data/fft_test_%02d%c.txt', phase-1, 'a'+t-1), opmod);
		    fprintf(fid, '%d %d\n', a);
		    fclose(fid);

%             % output in float format
% 			a = yy(phase:N_Parallel:end, idx((t-1)*512+(1:512))); 
% 			a = reshape(a.', 1, []); a = [real(a);imag(a)];
% 			
% 			fid = fopen(sprintf('../aie_src/data/fft_gold_%02d%c.txt', phase-1, 'a'+t-1), opmod);
% 			fprintf(fid, '%f %f\n', a);
% 			fclose(fid);
		end
	end

end

%% aie model ssca
out = aie_model_ssca_fp(tpwx_dat, 0);

for k=1:N_symb

	if(k==1)
		opmod = 'wt';
	else
		opmod = 'at';
	end


    xx = reshape(out((k-1)*N_datablk + (1:N_datablk)), 1024, []);
	aa2 = reshape(xx.', 8, []);
	
	for phase=1:N_Parallel
	
		for t=1:2
		% input in int32 format
		a = aa2( (t-1)*4 + (1:4), :);
		a = reshape(a, 1, []); a = [real(a);imag(a)]; a = float2int(a);

		fid = fopen(sprintf('../aie/data/fft_golden_%02d%c.txt', phase-1, 'a'+t-1), opmod);
		fprintf(fid, '%d %d\n', a);
		fclose(fid);
		end
	end

end


% % save mid-output
% for k=1:numel(dbinfo)
% 
% 	if(k==1)
% 		opmod = 'wt';
% 	else
% 		opmod = 'at';
% 	end
% 
% 	x2 = dbinfo(k).x2;
% 	
% 	for phase=1:N_Parallel
% 	
% 		aa2 = x2(phase:N_Parallel:end, :); aa2 = reshape(aa2.', 8, []);
% 		
% 		for t=1:2
% 		
% 			a = aa2( (t-1)*4 + (1:4), :);
% 			a = reshape(a, 1, []); a = [real(a);imag(a)];
% 			
% 			fid = fopen(sprintf('../aie_src/data/fft_mid_out_%02d%c.txt', phase-1, 'a'+t-1), opmod);
% 			fprintf(fid, '%f %f\n', a);
% 			fclose(fid);
% 		end
% 	end
% 	
% end



% % save mid-input
% for k=1:numel(dbinfo)
% 
% 	if(k==1)
% 		opmod = 'wt';
% 	else
% 		opmod = 'at';
% 	end
% 
% 	x2 = dbinfo(k).x2;
% 	
% 	for phase=1:N_Parallel
% 	
% 		for t=1:2
% 		
% 			a = x2(t:2:end, phase:N_Parallel:end);
% 			a = reshape(a, 1, []); a = [real(a);imag(a)]; a = float2int(a);
% 			
% 			fid = fopen(sprintf('../aie_src/data/fft_mid_in_%02d%c.txt', phase-1, 'a'+t-1), opmod);
% 			fprintf(fid, '%d %d\n', a);
% 			fclose(fid);
% 
% 		end
% 	end
% 	
% end


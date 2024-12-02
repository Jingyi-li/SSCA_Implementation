M = 1024;
tt = 0:1023;
tt = kron(tt, tt');
t = single(exp( -1i * 2 * pi / (1024*1024) .* tt));
r_matrix = complex(single(zeros(M*24,1)));

for im = 1:M
    start_idx = (im-1)*24+1;
    r_matrix(start_idx:start_idx+7,1) = t(im,1:8);
    r_matrix(start_idx+8:start_idx+8+3,1) = single(exp( -1i * 2 * pi / (1024*1024) .* [8,16,24,32]*(im-1)));
    r_matrix(start_idx+12:start_idx+12+3,1) = single(exp( -1i * 2 * pi / (1024*1024) .* [40,48,56,64]*(im-1)));
    r_matrix(start_idx+16:start_idx+16+3,1) = single(exp( -1i * 2 * pi / (1024*1024) .* [64,128,192,256]*(im-1)));
    r_matrix(start_idx+20:start_idx+20+3,1) = single(exp( -1i * 2 * pi / (1024*1024) .* [256,512,768,1024]*(im-1)));

    % for i = 0:23
    %     % a = float2int(single(r_matrix(start_idx+i,1))); fprintf('%d, %d \n',real(a), imag(a));
    %     % if mod(i,4)==3
    %     %     fprintf('\n');
    %     % end
    % end
end
% a = reshape(r_matrix, 1, []); a = [real(a);imag(a)]; a = float2int(a);

% fid = fopen(sprintf('../aie/data/fft_rotate_coeff.txt'), 'wt');
% fprintf(fid, '%d %d\n', a);
% fclose(fid);

% ta = reshape(t(:,1:8), 1, []); ta = [real(ta);imag(ta)]; ta = float2int(ta);

% fid = fopen(sprintf('../aie/data/fft_rotate_coeff_test.txt'), 'wt');
% fprintf(fid, '%d %d\n', ta);
% fclose(fid);

%%
a = reshape(r_matrix, 1, []); a = [real(a);imag(a)]; a = float2int(a);
filename = 'rotate_coeff.h';
fileID = fopen(filename, 'w');
content = sprintf([ ...
    '//\n'...
    '// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.\n' ...
    '// SPDX-License-Identifier: MIT\n' ...
    '//\n\n' ...
    '#ifndef __ROTATE_COEFF_H__\n' ...
    '#define __ROTATE_COEFF_H__\n\n' ...
    'const int RTEXP[49152]{  ']);
% Write content to file
fprintf(fileID, '%s', content);


for i = 1:length(a)*2
    if i == length(a)*2
        fprintf(fileID, '%12d', a(i)); % No comma for the last element
    else
        fprintf(fileID, '%12d,', a(i));
    end
end


content = sprintf([ ...
    '};'...
    '\n\n' ...
    '#endif // __ROTATE_COEFF_H__ \n']);
fprintf(fileID, '%s', content);
% Close the file
fclose(fileID);
%%
%  im = 33:33
% 1065353216,           0,  
% 1065353216, -1186394149,  
% 1065353215, -1178005542,  
% 1065353213, -1172911133,  
% 
% 1065353211, -1169616935,  
% 1065353208, -1166322737,  
% 1065353205, -1164522526,  
% 1065353201, -1162875428,  
% 
% 1065353196, -1161228331,  
% 1065353137, -1152839738,  
% 1065353038, -1147745343,  
% 1065352900, -1144451192,  
% 
% 1065352723, -1141157072,  
% 1065352505, -1139356840,  
% 1065352249, -1137709822,  
% 1065351953, -1136062832,  
% 
% 1065351953, -1136062832,  
% 1065348163, -1127675216,  
% 1065341847, -1122581716,  
% 1065333007, -1119290576,  
% 
% 1065333007, -1119290576,  
% 1065272429, -1110917834,  
% 1065171628, -1105837949,  
% 1065030846, -1102592574);
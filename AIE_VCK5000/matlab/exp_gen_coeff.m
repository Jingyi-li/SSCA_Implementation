% % Generate chebwin window coeff
Np = 64;
exp_coeff = single(exp(-j*2*pi*[0:63]/64));

filename = 'exp_coeff.h';
fileID = fopen(filename, 'w');
exp_coeff = reshape(exp_coeff.', 1, []); exp_coeff = [real(exp_coeff);imag(exp_coeff)]; exp_coeff_int = float2int(exp_coeff);
exp_coeff_int = reshape(exp_coeff_int, 1, []);

content = sprintf([ ...
    '//\n'...
    '// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.\n' ...
    '// SPDX-License-Identifier: MIT\n' ...
    '//\n\n' ...
    '#ifndef __EXP_COEFF_H__\n' ...
    '#define __EXP_COEFF_H__\n\n' ...
    '#define DCEXP {  ']);
% Write content to file
fprintf(fileID, '%s', content);
for i = 1:length(exp_coeff_int)
    if i == length(exp_coeff_int)
        fprintf(fileID, '%12d', exp_coeff_int(i)); % No comma for the last element
    else
        fprintf(fileID, '%12d,', exp_coeff_int(i));
    end
end

content = sprintf([ ...
    '}'...
    '\n\n' ...
    '#endif // __EXP_COEFF_H__ \n']);
fprintf(fileID, '%s', content);
% Close the file
fclose(fileID);

% Confirm file creation
disp(['File ', filename, ' has been created.']);
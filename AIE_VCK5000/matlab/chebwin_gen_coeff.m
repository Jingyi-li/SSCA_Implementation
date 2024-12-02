% % Generate chebwin window coeff
Np = 64;
window = single(chebwin(Np));

filename = 'win_coeff.h';
fileID = fopen(filename, 'w');
windowint = float2int(window);
content = sprintf([ ...
    '//\n'...
    '// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.\n' ...
    '// SPDX-License-Identifier: MIT\n' ...
    '//\n\n' ...
    '#ifndef __WIN_COEFF_H__\n' ...
    '#define __WIN_COEFF_H__\n\n' ...
    '#define CHEBWIN {  ']);
% Write content to file
fprintf(fileID, '%s', content);
for i = 1:length(windowint)
    if i == length(windowint)
        fprintf(fileID, '%12d', windowint(i)); % No comma for the last element
    else
        fprintf(fileID, '%12d,', windowint(i));
    end
end

content = sprintf([ ...
    '}'...
    '\n\n' ...
    '#endif // __WIN_COEFF_H__ \n']);
fprintf(fileID, '%s', content);
% Close the file
fclose(fileID);

% Confirm file creation
disp(['File ', filename, ' has been created.']);
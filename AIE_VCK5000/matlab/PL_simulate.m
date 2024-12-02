

M = 1024;
Np = 64;
LANE = 8;

abuff = zeros(M,Np+LANE);
% move from memin to URAM
for idx = 0:M/LANE-1
    for ic = 0:M-1
        for ir = 0:Np/LANE
            tempdata = idx + ic*(M/LANE)+ir;
            for il = 0:LANE
                abuff(ic+1,ir*LANE+il+1) = tempdata*LANE+il;
            end
        end
    end
end

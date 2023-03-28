function netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qt)

% this can be revised (Naveen)
% after perturbation you can not have these
% [2 2 2 -1 0] or [-2 -2 -2 1 0]
%however this is ok
% [-2 -2 -3 0] or [2 2 3 0]

diffVec = netPosAcct2 - netPosAcct;
if scale>0 && sum(diffVec<0)>0
    % last quantity was a BUY of qt contracts and net position should not
    % decrease
    netPosAcct2(diffVec<0) = netPosAcct(diffVec<0);
    while sum(netPosAcct2-netPosAcct) ~= scale*qt
        tmp = netPosAcct2-netPosAcct;
        indexJ = find(tmp == max(tmp));
        if length(indexJ) > 1
            %disp([sum(netPosAcct2-netPosAcct) scale*qt]);
        end
        netPosAcct2(indexJ(1)) = netPosAcct2(indexJ(1))-1;
    end
elseif scale<0  && sum(diffVec>0)>0
    % last quantity was a SHORT of qt contracts and net position should not
    % increase
    netPosAcct2(diffVec>0) = netPosAcct(diffVec>0);
    while sum(netPosAcct2-netPosAcct) ~= scale*qt
        tmp = netPosAcct2-netPosAcct;
        indexJ = find(tmp == min(tmp));
        if length(indexJ) > 1
            %disp([sum(netPosAcct2-netPosAcct) scale*qt]);
        end
        netPosAcct2(indexJ(1)) = netPosAcct2(indexJ(1))+1;
    end
end
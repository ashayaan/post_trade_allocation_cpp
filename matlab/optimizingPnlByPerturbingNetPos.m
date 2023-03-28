function parSet = optimizingPnlByPerturbingNetPos(parSet)

netPosAcct2  = parSet.netPosAcct2;
sgnNet       = parSet.sgnNet;
af           = parSet.af;
cumPnl       = parSet.cumPnl;
pnlAcctSoFar = parSet.pnlAcctSoFar;
netPosAcct   = parSet.netPosAcct;
scale        = parSet.scale;
qt           = parSet.qt;
pricePre     = parSet.pricePre;
priceNow     = parSet.priceNow;
onePtValue   = parSet.onePtValue;
perturbVec   = parSet.perturbVec;
nAccts = length(af);
par = abs(netPosAcct2);
flag = 0;
netSoFar = parSet.netSoFar;

% disp(['start point: ' num2str(par)]);

if length(perturbVec) == 2
    xi = perturbVec(1);
    xj = perturbVec(2);
    for i = 1:nAccts
        for j = 1:nAccts
            if i~=j
                actionVector = zeros(1,nAccts);
                actionVector(i) = xi;
                actionVector(j) = xj;
                par1 = par + actionVector;  % e.g. [2 3 2 0] + [-1 0 1 0]

                if sum(par1<0) == 0  % Naveen
%                   disp(par1)
                    netPosAcct2 = sgnNet*par1;
                    netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qt);
                    pnlAcctSoFar2 = pnlAcctSoFar + netPosAcct2*(priceNow-pricePre)*onePtValue;
                    mae = criterion(pnlAcctSoFar2, cumPnl, af, '1');
                    %disp([num2str(netPosAcct2) '   ' num2str(mae)])
                    if mae == parSet.minMae
                        e1 = entropy(netPosAcct2./af);
                        e2 = entropy(netSoFar./af);
                        if e1 < e2
                            flag = 1;
                            %disp('lower entropy');
                        end
                    end

                    if mae < parSet.minMae || flag
                        parSet.minMae = mae;
                        parSet.optimalCase = [i j actionVector(i) actionVector(j)];
                        qtAcct = netPosAcct2 - netPosAcct;
                        parSet.optimalAllocation = abs(qtAcct);
                        netSoFar = netPosAcct2;
                        flag = 0;
                    end
                end
            end
        end
    end

elseif length(perturbVec) == 3 && nAccts >=3
    
    xi = perturbVec(1);
    xj = perturbVec(2);
    xk = perturbVec(3);
    for i = 1:nAccts
        for j = 1:nAccts
            for k = 1:nAccts
                if i~=j && i~= k && j~=k
                    actionVector = zeros(1,nAccts);
                    actionVector(i) = xi;
                    actionVector(j) = xj;
                    actionVector(k) = xk;
                    par1 = par + actionVector;
                    if sum(par1<0) == 0
                        %disp(par1)
                        netPosAcct2 = sgnNet*par1;
                        netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qt);
                        pnlAcctSoFar2 = pnlAcctSoFar + netPosAcct2*(priceNow-pricePre)*onePtValue;
                        mae = criterion(pnlAcctSoFar2, cumPnl, af, '1');
                        if mae == parSet.minMae
                            e1 = entropy(netPosAcct2./af);
                            e2 = entropy(netSoFar./af);
                            if e1 < e2
                                flag = 1;
                                %disp('lower entropy');
                            end
                            if e1 == e2
                                %                                 disp('same entropy');
                            end
                        end
                        if mae < parSet.minMae  || flag
                            parSet.minMae = mae;
                            parSet.optimalCase = [i j k actionVector(i) actionVector(j) actionVector(k)];
                            qtAcct = netPosAcct2 - netPosAcct;
                            parSet.optimalAllocation = abs(qtAcct);
                            netSoFar = netPosAcct2;
                            flag = 0;
                        end
                    end
                end
            end
        end
    end

elseif length(perturbVec) == 4 && nAccts >= 4
    xi = perturbVec(1);
    xj = perturbVec(2);
    xk = perturbVec(3);
    xl = perturbVec(4);
    for i = 1:nAccts
        for j = 1:nAccts
            for k = 1:nAccts
                for l = 1:nAccts
                    if i~=j && i~=k && i~=l && j~=k && j~=l && k~=l
                        actionVector = zeros(1,nAccts);
                        actionVector(i) = xi;
                        actionVector(j) = xj;
                        actionVector(k) = xk;
                        actionVector(l) = xl;
                        par1 = par + actionVector;
                        if sum(par1<0) == 0
                            %disp(par1)
                            netPosAcct2 = sgnNet*par1;
                            netPosAcct2 = sanityCheckOnNetPosAcct(netPosAcct, netPosAcct2, scale, qt);
                            pnlAcctSoFar2 = pnlAcctSoFar + netPosAcct2*(priceNow-pricePre)*onePtValue;
                            mae = criterion(pnlAcctSoFar2, cumPnl, af, '1');
                            if mae == parSet.minMae
                                e1 = entropy(netPosAcct2./af);
                                e2 = entropy(netSoFar./af);
                                if e1 < e2
                                    flag = 1;
                                    %disp('lower entropy');
                                end
                                if e1 == e2
                                    %                                 disp('same entropy');
                                end
                            end
                            if mae < parSet.minMae  || flag
                                parSet.minMae = mae;
                                parSet.optimalCase = [i j k actionVector(i) actionVector(j) actionVector(k)];
                                qtAcct = netPosAcct2 - netPosAcct;
                                parSet.optimalAllocation = abs(qtAcct);
                                netSoFar = netPosAcct2;
                                flag = 0;
                            end
                        end
                    end
                end
            end
        end
    end
end


parSet.netSoFar = netSoFar;
%





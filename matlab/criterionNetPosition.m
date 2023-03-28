function mae = criterionNetPosition(netP, af)

nAccts = length(netP);
mae = 0;
for i = 1:nAccts
    for j = 1:nAccts
        if i~=j
            mae = mae+(netP(i)/af(i)-netP(j)/af(j))^2;
        end
    end
end
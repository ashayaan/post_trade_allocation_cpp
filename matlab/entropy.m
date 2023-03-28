function a = entropy(data)

N = length(data);
a = 0.0;
for i = 1:N
    for j = i+1:N
        a = a + (data(i)-data(j))^2;
    end
end








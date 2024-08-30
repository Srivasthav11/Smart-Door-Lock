% Create a MATLAB GUI for sending a password via UART

% Create a figure and components
f = figure('Name', 'Password Sender', 'Position', [200, 200, 300, 150]);

% Input field for password
editPassword = uicontrol('Style', 'edit', 'Position', [50, 80, 200, 30]);

% Button to send password
btnSend = uicontrol('Style', 'pushbutton', 'String', 'Send Password', ...
    'Position', [100, 30, 100, 30], 'Callback', {@sendPassword, editPassword});

% Function to send password
function sendPassword(~, ~, editPassword)
    % Get password from the input field
    password = get(editPassword, 'String');
    
    % Check if the password is exactly 4 digits
    if length(password) ~= 4 || ~all(isstrprop(password, 'digit'))
        errordlg('Password must be 4 digits.', 'Invalid Password', 'modal');
        return;
    end
    
    % Open the UART communication
    try
        s = serialport('COM7', 9600); % Change COM port and baud rate as needed
        configureTerminator(s, "LF");
        fopen(s);
    catch
        errordlg('Failed to open serial port.', 'Error', 'modal');
        return;
    end
    
    % Send the password
    try
        write(s, password, "char");
    catch
        errordlg('Failed to send password.', 'Error', 'modal');
    end
    
    % Close the UART communication
    try
        fclose(s);
        delete(s);
    catch
        errordlg('Failed to close serial port.', 'Error', 'modal');
    end
end

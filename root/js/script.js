let token = '';

async function getToken(event) {
    event.preventDefault();
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    console.log('Form submitted with username:', username, 'password:', password);


    const xhr = new XMLHttpRequest();
    xhr.open('POST', '/', true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');


    xhr.onreadystatechange =function() {
        console.log('OK');
        if (xhr.readyState === 4) {
            console.log('Request completed with status:', xhr.status);
            if (xhr.status === 200) {
                token = xhr.responseText;
                alert('Load succeed');
                
            } else {
                
                console.error('Error in request:', xhr.status, xhr.statusText);
            }
        }
    };


    const data = `username=${encodeURIComponent(username)}&password=${encodeURIComponent(password)}`;
    
    console.log('Sending data:', data);
    xhr.send(data);
}

async function accessProtected() {
    if (!token) {
        alert('Please get a token first');
        return;
    }

    const response = await fetch('http://localhost:9001/secured', {
        headers: {
            'Authorization': 'Bearer ' + token
        }
    });

    if (response.ok) {
        const newWindow = window.open();
        const htmlContent = await response.text();
        newWindow.document.open();
        newWindow.document.write(htmlContent);
        newWindow.document.close();
    } else {
        alert('You have no qualification');
    }
}


document.getElementById('loadForm').addEventListener('submit', getToken);
document.addEventListener('DOMContentLoaded', () => {
    get_personnel();
  });

function get_personnel(){
    // Инициализировать новый запрос
          var request = new XMLHttpRequest();
          request.open('GET', '/getPersonnel');

          // Функция обратного вызова, когда запрос завершен
          request.onload = () => {
              // Извлечение данных JSON из запроса



              const data = JSON.parse(request.responseText);
              fillTable(data);
          }

          // Послать запрос
          request.send(null);
}

function fillTable(data){
    var table = document.getElementById("table");
    var text = "<tr><td>Id</td><td>Name</td><td>Number</td><td>Speciality</td></tr>";
    for(let i = 0; i < data.length; i++){
        let id = data[i].personnel.id;
        let name = data[i].personnel.name;
        let number = data[i].personnel.number;
        let speciality = data[i].personnel.speciality;
        text += "<tr>" +
                    `<td>${id}</td>` +
                    "<td>" +
                        `<input type=\"text\"  id=\"name${id}_input\" value=\"${name}\">` +
                    "</td>" +
                    "<td>" +
                        `<input type=\"text\"  id=\"number${id}_input\" value=\"${number}\">` +
                    "</td>" +
                    "<td>" +
                        `<input type=\"text\"  id=\"speciality${id}_input\" value=\"${speciality}\">` +
                    "</td>" +
                    "<td>" +
                        `<a  onclick=\"update_personnel(${id})\">Update</a>` +
                    "</td><td>" + 
                        `<a onclick=\"remove_personnel(${id})\">Remove</a>` +
                    "</td></tr>";
    }
    text += "<tr><td></td><td><input type=\"text\"  id=\"name_input\">" +
            "</td><td><input type=\"text\"  id=\"number_input\">" +
            "</td><td><input type=\"text\"  id=\"speciality_input\">" +
            "</td><td><a  onclick=\"create_personnel()\">Add</a>" +
            "</td><td></td></tr>"

    table.innerHTML = text;
}

function remove_personnel(id){
    var request = new XMLHttpRequest();
    request.open("POST", '/deletePersonnel?id=' + id);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
          response = JSON.parse(request.responseText);
          if(request.status == '200'){
            get_books();
            WriteMessage(response.success, true);
          }
          else{
            WriteMessage(response.error, false);
          }
      }

    request.send();
}

function create_personnel(){
    var request = new XMLHttpRequest();
    var name = document.getElementById("name_input").value;
    var number = document.getElementById("number_input").value;
    var speciality = document.getElementById("speciality_input").value;
    request.open("POST", `/createPersonnel?name=${name}&number=${number}&speciality=${speciality}`);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
          response = JSON.parse(request.responseText);
          if(request.status == '200'){
            get_personnel();
            WriteMessage(response.success, true);
          }
          else{
            WriteMessage(response.error, false);
          }
      }

    request.send();
}

function update_personnel(id){
    var request = new XMLHttpRequest();
    var name = document.getElementById(`name${id}_input`).value;
    var number = document.getElementById(`number${id}_input`).value;
    var speciality = document.getElementById(`speciality${id}_input`).value;
    request.open("POST", `/updatePersonnel?id=${id}&name=${name}&number=${number}&speciality=${speciality}`);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
          response = JSON.parse(request.responseText);
          if(request.status == '200'){
            get_personnel();
            WriteMessage(response.success, true);
          }
          else{
            WriteMessage(response.error, false);
          }
      }

    request.send();
}

function WriteMessage(message, success){
    var now = new Date();
    status_text = document.getElementById("answertext");
    status_text.style.color = success ? "green" : "red";
    Year = now.getFullYear();
    Month = now.getMonth();
    Day = now.getDate();
    Hour = now.getHours();
    Minutes = now.getMinutes();
    Seconds = now.getSeconds();
    status_text.innerHTML = `${Year}.${Month}.${Day} ${Hour}:${Minutes}:${Seconds} - ${message}`;
}
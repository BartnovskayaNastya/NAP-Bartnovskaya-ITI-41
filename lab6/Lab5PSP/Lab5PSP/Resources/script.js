function Send(){
    let a = document.getElementById('number_a').value;
    let b = document.getElementById('number_b').value;
	let result = document.getElementById('result');
    var params = a + ' ' + b;
	var request = new XMLHttpRequest();
    request.open("POST", '/' + params);

    // Функция обратного вызова, когда запрос завершен
      request.onload = () => {
		  result.innerHTML = request.responseText;
      }

    request.send();
}
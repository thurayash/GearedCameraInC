function openModal(x) {
    console.log(x.src);
    document.getElementById('srcModal').src = x.src;
    document.getElementById('launchModal').classList.add('is-active');
}

function closeModal() {
  document.getElementById('myModal').style.display = "none";
}


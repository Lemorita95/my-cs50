document.addEventListener('DOMContentLoaded', function() {

    const color = document.querySelector('.img-1');
    const colorInput = document.querySelector('.color-input-shirt');

    colorInput.addEventListener('input', () => {
        color.style.background = colorInput.value;
    })

});

document.addEventListener('DOMContentLoaded', function() {

    const color = document.querySelector('.img-2');
    const colorInput = document.querySelector('.color-input-pant');

    colorInput.addEventListener('input', () => {
        color.style.background = colorInput.value;
    })

});

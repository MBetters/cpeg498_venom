$(document).ready(function() {
    
    $.get("/info", function(data) {
        var action = data["action"];
        console.log("action: " + action);
        $("input[name=action_form_radio_button]").each(function(index) {
            console.log(index + ": " + $(this).val());
            var input_val = $(this).val();
            if (input_val == action) {
                $(this).prop("checked", true);
            }
        });
        $("#latest_action").html(data["action"]);
    });
    
    $("#action_form_submit_button").click(function(event) {
        //get the value of the first input that has name=action_form_radio_button
        //and checked=true
        var action = $('input[name=action_form_radio_button]:checked').val();
        //console.log(action);
        $.post({
            url: "/update_action",
            data: {"action": action},
            success: function(data, textStatus, jqXHR) {
                console.log("action update successful");
                console.log(data);
                console.log(textStatus);
                console.log(jqXHR);
                $("#latest_action").html(action);
            }
        });
    });
    
    //alert($("input[name='action']:checked").val());
    
    return;
});

// Setup editors
function setupInfoArea(id) {
  const e = ace.edit(id);
  e.setShowPrintMargin(false);
  e.setOptions({
    readOnly: true,
    highlightActiveLine: false,
    highlightGutterLine: false
  })
  e.renderer.$cursorLayer.element.style.opacity=0;
  return e;
}

function setupEditorArea(id, lsKey) {
  const e = ace.edit(id);
  e.setShowPrintMargin(false);
  e.setValue(localStorage.getItem(lsKey) || '');
  e.moveCursorTo(0, 0);
  return e;
}

const grammarEditor = setupEditorArea("grammar-editor", "grammarText");
grammarEditor.getSession().setMode("ace/mode/yaml");
const codeEditor = setupEditorArea("code-editor", "codeText");

const codeAst = setupInfoArea("code-ast");
const codeLexer = setupInfoArea("code-lexer");

function loadLalr_sample(self) {
  let base_url = "./"
  switch(self.options[self.selectedIndex].value) {
    case "Json parser":
      $.get(base_url + "json3.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.json.txt", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/json");
      });
      break;
    case "Lua parser":
      $.get(base_url + "lua.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.lua", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/lua");
      });
      break;
      case "Carbon parser":
      $.get(base_url + "carbon-lang.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "prelude.carbon", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/typescript");
      });
      break;
      case "Postgresql parser (be patient)":
      $.get(base_url + "postgresql-16.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.sql", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/sql");
      });
      break;
      case "Postgresql parser case insensitive (be patient)":
      $.get(base_url + "postgresql-16-nc.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.sql", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/sql");
      });
      break;
      case "C++ parser (bug)":
      $.get(base_url + "cxx-parser.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.cpp", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/c_cpp");
      });
      break;
      case "Linden Script parser":
      $.get(base_url + "lsl_ext.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.lsl", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/text");
      });
      break;
      case "LALR parser (not working)":
      $.get(base_url + "lalr.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "lalr.g", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/yaml");
      });
      break;
      case "Bison parser (not working)":
      $.get(base_url + "bison.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "carbon-lang.y", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/yaml");
      });
      break;
      case "DParser parser (not working)":
      $.get(base_url + "dparser.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.dparser", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/yaml");
      });
      break;
      case "Parse_gen parser (not working)":
      $.get(base_url + "parse_gen.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.parse_gen", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/yaml");
      });
      break;
      case "Tameparse parser (not working)":
      $.get(base_url + "tameparser.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.tameparser", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/yaml");
      });
      break;
      case "Javascript parser (not working)":
      $.get(base_url + "javascript.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.js.txt", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/javascript");
      });
      break;
      case "JavascriptCore parser":
      $.get(base_url + "javascript-core.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.js.txt", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/javascript");
      });
      break;
      case "Ansi C parser":
      $.get(base_url + "cparser.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.c", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/c_cpp");
      });
      break;
      case "Ispc parser":
      $.get(base_url + "ispc.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.ispc", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/c_cpp");
      });
      break;
      case "Java11 parser":
      $.get(base_url + "java11.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.java", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/java");
      });
      break;
      case "Rust parser":
      $.get(base_url + "rust.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.rs", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/rust");
      });
      break;
      case "Go parser (not working)":
      $.get(base_url + "go.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.go", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/golang");
      });
      break;
      case "PHP-8.2 parser (not working)":
      $.get(base_url + "php-8.2.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.php", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/php");
      });
      break;
      case "Gringo/Clingo non grounded parser":
      $.get(base_url + "gringo-ng.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.clingo", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/text");
      });
      break;
      case "Ada parser":
      $.get(base_url + "ada-adayacc.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.adb", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/ada");
      });
      break;
      case "Ada parser case insensitive":
      $.get(base_url + "ada-adayacc-nc.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.adb", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/ada");
      });
      break;
      case "XML parser":
      $.get(base_url + "xml.g", function( data ) {
        grammarEditor.setValue( data );
      });
      $.get(base_url + "test.xml.txt", function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode("ace/mode/xml");
      });
      break;
  }
}

//$('#ast-mode').val(localStorage.getItem('optimizationMode') || '2');
$('#auto-refresh').prop('checked', localStorage.getItem('autoRefresh') === 'true');
$('#parse').prop('disabled', $('#auto-refresh').prop('checked'));

// Parse
function escapeHtml(unsafe) {
  return unsafe
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;");
}

function nl2br(str) {
  return str.replace(/\n/g, '<br>\n')
}

function textToErrors(str) {
  let errors = [];
  var regExp = /([^\n]+?)\n/g, match;
  while (match = regExp.exec(str)) {
    let msg = match[1];
    let line_col = msg.match(/^lalr \((\d+):(\d+)\):/);
    if (line_col) {
      errors.push({"ln": line_col[1], "col":line_col[2], "msg": msg});
    } else {
      errors.push({"msg": msg});
    }
  }
  return errors;
}

function generateErrorListHTML(errors) {
  let html = '<ul>';

  html += $.map(errors, function (x) {
    if (x.ln > 0) {
      return '<li data-ln="' + x.ln + '" data-col="' + x.col +
        '"><span>' + escapeHtml(x.msg) + '</span></li>';
    } else {
      return '<li><span>' + escapeHtml(x.msg) + '</span></li>';
    }
  }).join('');

  html += '<ul>';

  return html;
}

function updateLocalStorage() {
  localStorage.setItem('grammarText', grammarEditor.getValue());
  localStorage.setItem('codeText', codeEditor.getValue());
  //localStorage.setItem('optimizationMode', $('#opt-mode').val());
  localStorage.setItem('autoRefresh', $('#auto-refresh').prop('checked'));
}

var parse_start_time = 0;

function parse() {
  const $grammarValidation = $('#grammar-validation');
  const $grammarInfo = $('#grammar-info');
  const grammarText = grammarEditor.getValue();

  const $codeValidation = $('#code-validation');
  const $codeInfo = $('#code-info');
  const codeText = codeEditor.getValue();

  const astMode = $('#ast-mode').val();
  const generate_ebnf = $('#generate-action').val() == 'ebnf';
  const generate_yacc = $('#generate-action').val() == 'yacc';
  const lexer = $('#show-lexer').prop('checked');
  let generate_ast = $('#show-ast').prop('checked');
  if(generate_ast && astMode == 2)
	generate_ast = 2;

  $grammarInfo.html('');
  $grammarValidation.hide();
  $codeInfo.html('');
  $codeValidation.hide();
  codeAst.setValue('');
  codeLexer.setValue('');

  outputs.compile_status = '';
  outputs.parse_status = '';
  outputs.parse_stats = '';
  outputs.ast = '';
  outputs.lexer = '';
  outputs.parse_time = '';

  if (grammarText.length === 0) {
    return;
  }

  $('#overlay').css({
    'z-index': '1',
    'display': 'block',
    'background-color': 'rgba(0, 0, 0, 0.1)'
  });

  window.setTimeout(() => {
    parse_start_time = new Date().getTime();
    lalr_parse(grammarText, codeText, lexer, generate_ebnf, generate_yacc, generate_ast);

    $('#overlay').css({
      'z-index': '-1',
      'display': 'none',
      'background-color': 'rgba(1, 1, 1, 1.0)'
    });

    const isGenEBNF = result.parse == -3;
    if (result.compile == 0 || isGenEBNF) {
      $grammarValidation.removeClass('validation-invalid').show();

      codeLexer.insert(outputs.lexer);

      if (result.parse == 0) {
        $codeValidation.removeClass('validation-invalid').show();
      } else {
        $codeValidation.addClass('validation-invalid').show();
      }

      if(isGenEBNF) {
        $grammarInfo.html("<pre>" + escapeHtml(outputs.compile_status.replaceAll("'\\''", '"\'"')) + "</pre>");
        return;
      }

      if (outputs.parse_status.length > 0) {
        const errors = textToErrors(outputs.parse_status);
        const html = generateErrorListHTML(errors);
        $codeInfo.html(html);
      }
      else {
        //const parse_end_time = new Date().getTime();
        //$codeInfo.html('Execution time: ' + (parse_end_time - parse_start_time) + ' ms');
        $codeInfo.html(outputs.parse_time);
	$grammarInfo.html("<pre>" + outputs.parse_stats + "</pre>");
      }
    } else {
      $grammarValidation.addClass('validation-invalid').show();
    }

    if (outputs.compile_status.length > 0) {
      const errors = textToErrors(outputs.compile_status);
      const html = generateErrorListHTML(errors);
      $grammarInfo.html(html);
    }

    if (outputs.ast.length > 0) {
      codeAst.setValue(outputs.ast);
    }
  }, 0);
}

// Event handing for text editing
let timer;
function setupTimer() {
  clearTimeout(timer);
  timer = setTimeout(() => {
    updateLocalStorage();
    if ($('#auto-refresh').prop('checked')) {
      parse();
    }
  }, 200);
};
grammarEditor.getSession().on('change', setupTimer);
codeEditor.getSession().on('change', setupTimer);

// Event handing in the info area
function makeOnClickInInfo(editor) {
  return function () {
    const el = $(this);
    let line = el.data('ln') - 1;
    let col = el.data('col') - 1;
    editor.navigateTo(line, col);
    editor.scrollToLine(line, true, false, null);
    editor.focus();
  }
};
$('#grammar-info').on('click', 'li[data-ln]', makeOnClickInInfo(grammarEditor));
$('#code-info').on('click', 'li[data-ln]', makeOnClickInInfo(codeEditor));

// Event handing in the AST optimization
$('#opt-mode').on('change', setupTimer);
$('#generate-action').on('change', setupTimer);
$('#show-lexer').on('change', setupTimer);
$('#auto-refresh').on('change', () => {
  updateLocalStorage();
  $('#parse').prop('disabled', $('#auto-refresh').prop('checked'));
  setupTimer();
});
$('#parse').on('click', parse);

// Resize editors to fit their parents
function resizeEditorsToParent() {
  codeEditor.resize();
  codeEditor.renderer.updateFull();
  codeAst.resize();
  codeAst.renderer.updateFull();
  codeLexer.resize();
  codeLexer.renderer.updateFull();
}

// Show windows
function setupToolWindow(lsKeyName, buttonSel, codeSel, showDefault) {
  let storedValue = localStorage.getItem(lsKeyName);
  if (!storedValue) {
    localStorage.setItem(lsKeyName, showDefault);
    storedValue = localStorage.getItem(lsKeyName);
  }
  let show = storedValue === 'true';
  $(buttonSel).prop('checked', show);
  $(codeSel).css({ 'display': show ? 'block' : 'none' });

  $(buttonSel).on('change', () => {
    show = !show;
    localStorage.setItem(lsKeyName, show);
    $(codeSel).css({ 'display': show ? 'block' : 'none' });
    resizeEditorsToParent();
  });
}

setupToolWindow('show-ast', '#show-ast', '#code-ast', true);
setupToolWindow('show-lexer', '#show-lexer', '#code-lexer', false);

// Show page
$('#main').css({
  'display': 'flex',
});

// used to collect output from C
var outputs = {
  'default': '',
  'compile_status': '',
  'ast': '',
  'parse_status': '',
  'parse_stats': '',
  'parse_time': '',
};

// current output (key in `outputs`)
var output = "default";

// results of the various stages
var result = {
  'compile': 0,
  'parse': 0,
  'ast': 0,
  'lexer': 0,
};

// lalr_parse function: initialized when emscripten runtime loads
var lalr_parse = null;

// Emscripten
var Module = {

  // intercept stdout (print) and stderr (printErr)
  // note: text received is line based and missing final '\n'

  'print': function(text) {
    outputs[output] += text + "\n";
  },
  'printErr': function(text) {
    outputs[output] += text + "\n";
  },

  // called when emscripten runtime is initialized
  'onRuntimeInitialized': function() {
    // wrap the C `parse` function
    lalr_parse = cwrap('parse', 'number', ['string', 'string', 'number']);
    // Initial parse
    if ($('#auto-refresh').prop('checked')) {
      parse();
    }
  },
};

// vim: sw=2:sts=2

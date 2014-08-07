#![feature(globs)]

extern crate native;
extern crate glfw;
extern crate gl;

use glfw::Context;
use gl::types::*;

// Vertex data
static vertices: [GLfloat, ..6] = [
     0.0,  0.5,
     0.5, -0.5,
    -0.5, -0.5
];

// Shader sources
static vertex_src: &'static str =
   "#version 330\n\
    in vec2 position;\n\
    void main() {\n\
       gl_Position = vec4(position, 0.0, 1.0);\n\
    }";

static fragment_src: &'static str =
   "#version 330\n\
    out vec4 outColor;\n\
    void main() {\n\
       outColor = vec4(1.0, 1.0, 1.0, 1.0);\n\
    }";

#[start]
fn start(argc: int, argv: *const *const u8) -> int {
    native::start(argc, argv, main)
}

fn compile_shader(shader_type: GLenum, source: &str) -> Option<GLuint> {
    let shader = gl::CreateShader(shader_type);
    unsafe {
        // We pass in an array of pointers to c-strings as the third argument here, thus the
        // ugliness.
        gl::ShaderSource(shader, 1, &source.to_c_str().as_ptr(), std::ptr::null());
    }
    gl::CompileShader(shader);

    // Check if we suceeded
    let mut success = gl::FALSE as GLint;
    unsafe {
        gl::GetShaderiv(shader, gl::COMPILE_STATUS, &mut success);
    }
    if success == gl::TRUE as GLint {
        return Some(shader);
    }

    let mut len = 0 as GLint;
    unsafe {
        gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &mut len);
    }
    if len == 0 {
        return None;
    }

    // len including trailing null
    let mut s: Vec<u8> = Vec::with_capacity(len as uint - 1);
    unsafe {
        gl::GetShaderInfoLog(shader, len, &mut len as *mut GLsizei,
                             s.as_mut_slice().as_mut_ptr() as *mut GLchar);
        s.set_len(len as uint - 1);
    }

    println!("Error compiling shader: {}", String::from_utf8(s).ok().expect("non-utf8 infolog!"));

    None
}

fn main() {
    let glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();

    // Suggested Mac OS X compatible context hints from glfw-rs
    // (https://github.com/bjz/glfw-rs/blob/master/src/glfw/lib.rs#L554)
    glfw.window_hint(glfw::ContextVersion(3, 2));
    glfw.window_hint(glfw::OpenglForwardCompat(true));
    glfw.window_hint(glfw::OpenglProfile(glfw::OpenGlCoreProfile));

    let (window, _) = glfw.create_window(300, 300, "Hello this is window", glfw::Windowed)
        .expect("Failed to create GLFW window.");
    window.make_current();

    // This call needs a context (thus it comes after make_current)
    gl::load_with(|s| glfw.get_proc_address(s));

     
    // Generate 1 buffer
    let mut buf_id: GLuint = 0;
    unsafe {
        gl::GenBuffers(1, &mut buf_id);
    }
    gl::BindBuffer(gl::ARRAY_BUFFER, buf_id);
     
    unsafe {
        // Give our vertices to OpenGL.
        gl::BufferData(
            gl::ARRAY_BUFFER,
            (vertices.len() * std::mem::size_of::<GLfloat>()) as GLsizeiptr,
            std::mem::transmute(&vertices[0]),
            gl::STATIC_DRAW
        );
    }

    // Create and compile the vertex shader
    let vertex_shader = compile_shader(gl::VERTEX_SHADER, vertex_src)
        .expect("failed to compile vertex shader");

    let fragment_shader = compile_shader(gl::FRAGMENT_SHADER, fragment_src)
        .expect("failed to compile fragment shader");
    
    // Link the vertex and fragment shader into a shader program
    let shader_program = gl::CreateProgram();
    gl::AttachShader(shader_program, vertex_shader);
    gl::AttachShader(shader_program, fragment_shader);
    gl::LinkProgram(shader_program);

    // Check if we suceeded
    let mut success = gl::FALSE as GLint;
    unsafe {
        gl::GetProgramiv(shader_program, gl::LINK_STATUS, &mut success);
    }
    if success == gl::FALSE as GLint {
        println!("Failed to link.");
        return ();
    }

    unsafe {
        gl::BindFragDataLocation(shader_program, 0, "outColor".to_c_str().as_ptr());
    }

    gl::UseProgram(shader_program);
    
    let pos_attrib: GLuint;
    unsafe {
        pos_attrib = gl::GetAttribLocation(shader_program, "position".to_c_str().as_ptr()) as GLuint;
    }
    gl::EnableVertexAttribArray(pos_attrib);
    unsafe {
        gl::VertexAttribPointer(pos_attrib, 2, gl::FLOAT, gl::FALSE, 0, std::ptr::null());
    }

    while !window.should_close() {
        glfw.poll_events();

        gl::ClearColor(0.1, 0.1, 0.1, 1.0);
        gl::Clear(gl::COLOR_BUFFER_BIT);

        // 1rst attribute buffer : vertices
        // gl::BindBuffer(gl::ARRAY_BUFFER, buf_id);
         
        // Draw the triangle !
        gl::DrawArrays(gl::TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

        window.swap_buffers();
         
        // gl::DisableVertexAttribArray(0);
    }
}

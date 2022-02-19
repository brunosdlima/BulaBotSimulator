#pragma once
#include <GL/glew.h>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "glm/glm.hpp"
#include "stb_image/stb_image.h"

class VertexBuffer {
private:
   unsigned int m_RendererID;
public:
   VertexBuffer(const void* data, unsigned int size) { 
      glGenBuffers(1, &m_RendererID);  //How many buffers, pointer to unsigned int which will be the buffer (writes in this integer memory)
      //Every time you want to update position you must bind again (recopy next two functions)
      glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);//Select this as an array, Select the current buffer for usage
      glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); //Target as an array, size of the buffer, provided data (null for no data), usage
   };
   ~VertexBuffer() {
      glDeleteBuffers(1, &m_RendererID); //How many buffers to be deleted, adress of deleted buffers
      std::cout << "Vertex was destructed" << std::endl;
   };
   void Bind()const {
      glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);//Select this as an array, Select the current buffer for usage
   };
   void Unbind()const {
      glBindBuffer(GL_ARRAY_BUFFER, 0);//Select this as an array, Select the current buffer for usage
   };
};

struct VertexBufferElement {
   unsigned int type;
   unsigned int count;
   unsigned char normalized;
   static unsigned int GetSizeOfType(unsigned int type) {
      switch (type) {
      case GL_FLOAT: return sizeof(GL_FLOAT);
      case GL_UNSIGNED_INT: return sizeof(GL_UNSIGNED_INT);
      case GL_UNSIGNED_BYTE: return sizeof(GL_UNSIGNED_BYTE);
      }
      return 0;
   };
};

class VertexBufferLayout {
private:
   std::vector<VertexBufferElement>m_Elements;
   unsigned int m_Stride;
public:
   VertexBufferLayout() : m_Stride(0) {

   };
   ~VertexBufferLayout() {

   };

   template <typename T>
   void Push(unsigned int count) {
      static_assert(false);
   };
   template <>
   void Push<float>(unsigned int count) {
      m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
      m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
   };
   template <>
   void Push<unsigned int>(unsigned int count) {
      m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
      m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
   };
   template <>
   void Push<unsigned char>(unsigned int count) {
      m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
      m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
   };


   inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
   inline unsigned int GetStride() const { return m_Stride; }

};

class VertexArray {
private:
   unsigned int m_RendererID;
public:
   VertexArray() {
      glGenVertexArrays(1, &m_RendererID);
   };
   ~VertexArray() {
      glDeleteVertexArrays(1, &m_RendererID);
   };
   void Bind() const {
      glBindVertexArray(m_RendererID);
   };
   void Unbind() const {
      glBindVertexArray(0);
   };
   void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
      Bind();
      vb.Bind();
      unsigned int offset = 0;
      const auto& elements = layout.GetElements();
      for (unsigned int i = 0; i < elements.size(); i++) {
         const auto& element = elements[i];
         glEnableVertexAttribArray(i); //Enable vertex attribute fo usage
         //This function is called for each attribute
         glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);//Index for the current attribute, Size of the attribute (not in bytes), type of the data, normalize?, size between two vertex (stride), pointer to the beggining of the desired attribute
         offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
      }
   };
};

class IndexBuffer {
private:
   unsigned int m_RendererID;
   unsigned int m_Count;
public:
   IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count) {
      glGenBuffers(1, &m_RendererID);  //How many buffersindices, pointer to unsigned int which will be the buffer indices (writes in this integer memory)
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);//Select this as an array, Select the current buffer for usage
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW); //Target as an array, size of the buffer, provided data (null for no data), usage
   };
   ~IndexBuffer() {
      glDeleteBuffers(1, &m_RendererID); //How many buffers to be deleted, adress of deleted buffers
      std::cout << "Index was destructed" << std::endl;
   };
   void Bind() const {
      glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);//Select this as an array, Select the current buffer for usage
   };
   void Unbind() const {
      glBindBuffer(GL_ARRAY_BUFFER, 0);//Select this as an array, Select the current buffer for usage
   };
   inline unsigned int GetCount() const { return m_Count; };
};

struct ShaderProgramSource {
   std::string VertexSource;
   std::string FragmentSource;
};

class Shader {
private:
   unsigned int m_RendererID;
   std::string m_filepath;

   int GetUniformLocation(const std::string& name) {
      if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) { return m_UniformLocationCache[name]; }

      int location = glGetUniformLocation(m_RendererID, name.c_str()); //Get the location of the variable u_color to use in the next function
      if (location == -1) std::cout << "Warning : uniform "<< name << " does not exists!!!" << std::endl;
      return m_UniformLocationCache[name] = location;
      return location;
   }
   ShaderProgramSource ParseShader(const std::string filepath) { // This function reads the file and slipts it into different shaders
      std::ifstream stream(filepath); //Used to store the path of the file
      enum class shaderType {
         NONE = -1, VERTEX = 0, FRAGMENT = 1
      };
      std::string line;               //Used to read each line of the file
      std::stringstream ss[2];         //Used to read from file and store at shader
      shaderType type = shaderType::NONE;
      while (getline(stream, line)) { //Loop until it reaches the end of the file
         if (line.find("#shader") != std::string::npos) { // Checking if the line contains #shader (std::string::npos means invalid stream position)
            if (line.find("vertex") != std::string::npos) { // Checking if the line contains #shader
               type = shaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) { // Checking if the line contains #shader
               type = shaderType::FRAGMENT;
            }
         }
         else { // If the file is not setting up the type of the shader then it needs to be assigned to that shader
            ss[(int)type] << line << "\n";
         }
      }
      return { ss[0].str(), ss[1].str() };
   }
   unsigned int compileShader(unsigned int type, const std::string& source) { // This function inputs a code into a shader
      unsigned int id = glCreateShader(type); //Create a Shader and assign it to id
      const char* src = source.c_str(); //Go to the beggining of the source (argument of the function)
      glShaderSource(id, 1, &src, nullptr); //Shader, how many source codes, pointer to the actual pointer, length (null for the whole length of the string)
      glCompileShader(id); // Compile the current shader
      int result; //Used in order to check if compilation was succesfully in the next function
      glGetShaderiv(id, GL_COMPILE_STATUS, &result); // Check if compilation was sucessfully (iv stands fo integer and vector)
      if (result == GL_FALSE) {
         int length; //Used to get the size of the error message in next function
         glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //Store the size of the error message
         char* message = (char*)alloca(length * sizeof(char)); // Creates a char with the size of the length in order to get the error message (stored on the stack)
         glGetShaderInfoLog(id, length, &length, message); // Store the error message at message
         //The next two functions print which shader has failed
         if (type == GL_VERTEX_SHADER) std::cout << "Failed to compile vertex shader!!!" << std::endl;;
         if (type == GL_FRAGMENT_SHADER) std::cout << "Failed to compile fragment shader!!!" << std::endl;;
         std::cout << message << std::endl;
         glDeleteShader(id);
         return -1;
      }

      return id;
   }
   unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) { // Function used to create the shader
      unsigned int program = glCreateProgram(); // Returns an unsigned integer
      unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader); // Shader for the vertex
      unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader); // Shader for the fragment
      glAttachShader(program, vs); //Attach both shaders to current program in order to use it
      glAttachShader(program, fs); //Attach both shaders to current program in order to use it
      glLinkProgram(program);      //Program needs to be linked
      glValidateProgram(program);  //Program needs to be validated
      glDeleteShader(vs); //Delete intermediate shader for vertex
      glDeleteShader(fs); //Delete intermediate shader for fragment
      return program;
   }
   std::unordered_map<std::string, int> m_UniformLocationCache;
public:
   Shader(const std::string& filepath) : m_filepath(filepath), m_RendererID(0) {
      ShaderProgramSource source = ParseShader(filepath); // Testing program
      m_RendererID = CreateShader(source.VertexSource, source.FragmentSource); // Assign the code to current shader
   }
   ~Shader() {
      glDeleteProgram(m_RendererID);
   }

   void Bind() const {
      glUseProgram(m_RendererID);
   }
   void Unbind() const {
      glUseProgram(0);
   }

   //Set uniforms
   void setUniform1i(const std::string& name, int value) {
      glUniform1i(GetUniformLocation(name), value);
   }
   void setUniform1f(const std::string& name, float value) {
      glUniform1f(GetUniformLocation(name), value);
   }
   void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
      glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
   }
   void setUniformMat4f(const std::string& name, const glm::mat4& Matrix) {
      glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &Matrix[0][0]); //the second argument asks ow many matrix we are passing as argument, The third argument is asking if the matrix needs to be transposed
   }
};

class Renderer {
private:

public:
   Renderer() {

   }
   void Clear() const{
      glClear(GL_COLOR_BUFFER_BIT);
   }
   void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
      shader.Bind();
      va.Bind();
      ib.Bind();
      glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr); //Drawing triangles, number of indices, type of indices, pointer to the index buffer (remember to bind ibo then use nullptr)
   }

};

class Texture {
private:
   unsigned int m_RendererID;
   std::string m_filepath;
   unsigned char* m_LocalBuffer;
   int m_Width, m_Height, m_BPP; //BPP bits per pixel
   
public:
   Texture(const std::string& path) : m_RendererID(0), m_filepath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0){
      stbi_set_flip_vertically_on_load(1); //Function to flip PNG image
      m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

      glGenTextures(1, &m_RendererID);
      glBindTexture(GL_TEXTURE_2D, m_RendererID);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
      glBindTexture(GL_TEXTURE_2D, 0);
      if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
   }

   ~Texture() {
      glDeleteTextures(1, &m_RendererID);
   }
   void Bind(unsigned int slot = 0) const {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, m_RendererID);
   }
   void Unbind() const {
      glBindTexture(GL_TEXTURE_2D, 0);
   }
   inline int GetWidth() const { return m_Width; }
   inline int GetHeight() const { return m_Height; }
   inline int GetBPP() const { return m_BPP; }
};

class Input {
private:
   GLFWwindow* window;
   int button;
   int keycode;
public:
   
   Input(GLFWwindow* current_window) : window(current_window),button(0), keycode(0){
   }

   int GetButton() {
      return (*this).button;
   }

   bool IsKeyPressed(int keycode) {
      auto state = glfwGetKey(window, keycode);
      return state == GLFW_PRESS || state == GLFW_REPEAT;
   }
   bool IsMouseButtonPressed(int button) {
      auto state = glfwGetMouseButton(window, button);
      return state == GLFW_PRESS;
   }
   std::pair<float,float> GetMousePosition() {
      double mousex, mousey;
      glfwGetCursorPos(window, &mousex, &mousey);
      return { (float)mousex, (float)mousey };
   }
   float GetMouseX() {
      double mousex, mousey;
      glfwGetCursorPos(window, &mousex, &mousey);
      return (float)mousex;
   }
   float GetMouseY() {
      double mousex, mousey;
      glfwGetCursorPos(window, &mousex, &mousey);
      return (float)mousey;
   }
};
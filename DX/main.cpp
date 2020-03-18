//Include and link appropriate libraries and headers//
#include <Windows.h>
#include <iostream>
#include "winuser.h"
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXColors.h>
#include <dcomp.h>
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

//Global Declarations - Interfaces//
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;
ID3D11Buffer* squareIndexBuffer;
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;
ID3D11Buffer* squareVertBuffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* vertLayout;
ID3D11Buffer* cbPerObjectBuffer;

//Global Declarations - Others//
LPCTSTR WndClassName = L"firstwindow";
HWND hwnd = NULL;
HRESULT hr;

const int Width = 900;
const int Height = 900;

DirectX::XMMATRIX WVP;

XMMATRIX c_mercury_world;
XMMATRIX c_sun_world;
XMMATRIX c_venus_world;
XMMATRIX c_Earth_world;
XMMATRIX c_Mars_world;
XMMATRIX c_Upiter_world;
XMMATRIX c_Saturn_world;
XMMATRIX c_Uran_world;
XMMATRIX c_Neptune_world;
XMMATRIX c_Pluto_world;
XMMATRIX c_moon_world;

XMMATRIX camView;
XMMATRIX camProjection;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

XMMATRIX Rotation;
XMMATRIX Scale;
XMMATRIX Translation;
float rot = 0.01f;
float rot2 = 0.01f;
float rot3 = 0.01f;
float rot_merc = 0.01f;
float rot_venus = 0.01f;
float rot_earth = 0.01f;
float rot_mars = 0.01f;
float rot_upiter = 0.01f;
float rot_saturn = 0.01f;
float rot_neptune = 0.01f;
float rot_pluto = 0.01f;
float rot_moon = 0.01f;

//Function Prototypes//
bool InitializeDirect3d11App(HINSTANCE hInstance);
void CleanUp();
bool InitScene();
void UpdateScene();
void DrawScene();

bool InitializeWindow(HINSTANCE hInstance,
    int ShowWnd,
    int width, int height,
    bool windowed);
int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam);

//Create effects constant buffer's structure//
struct cbPerObject
{
    XMMATRIX  WVP;
};

cbPerObject cbPerObj;

//Vertex Structure and Vertex Layout (Input Layout)//
struct Vertex    //Overloaded Vertex Structure
{
    Vertex() {}
    Vertex(float x, float y, float z,
        float cr, float cg, float cb, float ca)
        : pos(x, y, z), color(cr, cg, cb, ca) {}

    XMFLOAT3 pos;
    XMFLOAT4 color;
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
    {
        MessageBox(0, L"Window Initialization - Failed",
            L"Error", MB_OK);
        return 0;
    }

    if (!InitializeDirect3d11App(hInstance))    //Initialize Direct3D
    {
        MessageBox(0, L"Direct3D Initialization - Failed",
            L"Error", MB_OK);
        return 0;
    }

    if (!InitScene())    //Initialize our scene
    {
        MessageBox(0, L"Scene Initialization - Failed",
            L"Error", MB_OK);
        return 0;
    }

    messageloop();

    CleanUp();

    return 0;
}

bool InitializeWindow(HINSTANCE hInstance,
    int ShowWnd,
    int width, int height,
    bool windowed)
{
    typedef struct _WNDCLASS {
        UINT cbSize;
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HANDLE hInstance;
        HICON hIcon;
        HCURSOR hCursor;
        HBRUSH hbrBackground;
        LPCTSTR lpszMenuName;
        LPCTSTR lpszClassName;
    } WNDCLASS;

    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WndClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Error registering class",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    hwnd = CreateWindowEx(
        NULL,
        WndClassName,
        L"Lesson 4 - Begin Drawing",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd)
    {
        MessageBox(NULL, L"Error creating window",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, ShowWnd);
    UpdateWindow(hwnd);

    return true;
}

bool InitializeDirect3d11App(HINSTANCE hInstance)
{
    //Describe our SwapChain Buffer
    DXGI_MODE_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    bufferDesc.Width = Width;
    bufferDesc.Height = Height;
    bufferDesc.RefreshRate.Numerator = 0;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //Describe our SwapChain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swapChainDesc.BufferDesc = bufferDesc;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //Create our SwapChain
    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
        D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

    //Create our BackBuffer
    ID3D11Texture2D* BackBuffer;
    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

    //Create our Render Target
    hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
    BackBuffer->Release();

    //Describe our Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = Width;
    depthStencilDesc.Height = Height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    //Create the Depth/Stencil View
    d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
    d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

    //Set our Render Target
    d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    return true;
}

void CleanUp()
{
    //Release the COM Objects we created
    SwapChain->Release();
    d3d11Device->Release();
    d3d11DevCon->Release();
    renderTargetView->Release();
    squareVertBuffer->Release();
    squareIndexBuffer->Release();
    VS->Release();
    PS->Release();
    VS_Buffer->Release();
    PS_Buffer->Release();
    vertLayout->Release();
    depthStencilView->Release();
    depthStencilBuffer->Release();
    cbPerObjectBuffer->Release();
}

bool InitScene()
{
    //Compile Shaders from shader file
    ID3DBlob* errorVertexCode;
    ID3DBlob* errorPixelCode;
	
    hr = D3DCompileFromFile(L"MiniTri.fx",
        nullptr ,
        nullptr ,
        "VS",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &VS_Buffer,
        &errorVertexCode);

    if (FAILED(hr)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorVertexCode) {
            char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
        }

        return false;
    }
	
    hr = D3DCompileFromFile(L"MiniTri.fx",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &PS_Buffer,
        &errorPixelCode);

    if (FAILED(hr)) {
        // If the shader failed to compile it should have written something to the error message.
        if (errorPixelCode) {
            char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

            std::cout << compileErrors << std::endl;
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, L"MiniTri.fx", L"Missing Shader File", MB_OK);
        }

        return false;
    }
	
    if (FAILED(hr))
    {

    }

    //Create the Shader Objects
    hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
    hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

    //Set Vertex and Pixel Shaders
    d3d11DevCon->VSSetShader(VS, 0, 0);
    d3d11DevCon->PSSetShader(PS, 0, 0);

    //Create the vertex buffer
    Vertex v[] =
    {
        Vertex(-1.0f, -1.0f, -1.0f, 0.1f, 0.1f, 0.1f, 1.0f),
        Vertex(-1.0f, +1.0f, -1.0f, 0.1f, 0.1f, 0.1f, 1.0f),
        Vertex(+1.0f, +1.0f, -1.0f, 0.1f, 0.1f, 0.1f, 1.0f),
        Vertex(+1.0f, -1.0f, -1.0f, 0.1f, 0.1f, 0.1f, 1.0f),

        Vertex(-1.0f, -1.0f, +1.0f, 1.0f, 0.388235331f, 0.278431386f, 1.0f),
        Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 0.388235331f, 0.278431386f, 1.0f),
        Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.388235331f, 0.278431386f, 1.0f),
        Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.388235331f, 0.278431386f, 1.0f),
    };

    DWORD indices[] = {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;

    iinitData.pSysMem = indices;
    d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);

    d3d11DevCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * 8;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;


    D3D11_SUBRESOURCE_DATA vertexBufferData;

    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = v;
    hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

    //Set the vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    d3d11DevCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);

    //Create the Input Layout
    hr = d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
         VS_Buffer->GetBufferSize(), &vertLayout);

    //Set the Input Layout
    d3d11DevCon->IASetInputLayout(vertLayout);

    //Set Primitive Topology
    d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Create the Viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Width;
    viewport.Height = Height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    //Set the Viewport
    d3d11DevCon->RSSetViewports(1, &viewport);

    //Create the buffer
    D3D11_BUFFER_DESC cbbd;
    ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

    cbbd.Usage = D3D11_USAGE_DEFAULT;
    cbbd.ByteWidth = sizeof(cbPerObject);
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.CPUAccessFlags = 0;
    cbbd.MiscFlags = 0;

    hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

    //Camera information
    camPosition = XMVectorSet(0.0f, 90.0f, -80.0f, 0.0f);
    camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //Set the View matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

    //Set the Projection matrix
    camProjection = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)Width / Height, 1.0f, 1000.0f);

    return true;
}

void UpdateScene()
{
    //Keep the cubes rotating
    rot += .0009f;
    if (rot > 6.26f)
        rot = 0.0f;

    rot2 += .001f;
    if (rot > 6.26f)
        rot = 0.0f;

    rot3 += .009f;
    if (rot > 6.26f)
        rot = 0.0f;

    rot_merc += .0024;
    if (rot_merc > 6.26f)
        rot_merc = 0.0f;

    XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR rotaxis2 = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

    XMMATRIX SelfRotation = XMMatrixRotationAxis(rotaxis, -rot3);
    XMMATRIX SelfRotation2 = XMMatrixRotationAxis(rotaxis, rot3);
    XMMATRIX Rotation3 = XMMatrixRotationAxis(rotaxis2, rot);

    //SUN
    c_sun_world = XMMatrixIdentity(); // обнуляем матрицу
    Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    c_sun_world = Translation * Rotation * Scale;

    //MERCURY
    c_mercury_world = XMMatrixIdentity();
    Translation = XMMatrixTranslation(0.0f, 0.0f, 7.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Scale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
    XMMATRIX SelfRotation_merc = XMMatrixRotationAxis(rotaxis, rot_merc);
    c_mercury_world = SelfRotation_merc * Translation * Rotation * Scale; // IT WORKS!

    //VENUS
    c_venus_world = XMMatrixIdentity();
    Translation = XMMatrixTranslation(0.0f, 0.0f, 10.0f);
    Rotation = XMMatrixRotationAxis(rotaxis2, -rot);
    Scale = XMMatrixScaling(0.4f, 0.4f, 0.4f);
    c_venus_world = SelfRotation2 * Translation * Rotation * Scale; // IT WORKS!

    //EARTH
    c_Earth_world = XMMatrixIdentity();
    Translation = XMMatrixTranslation(0.0f, 0.0f, 8.0f);
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Scale = XMMatrixScaling(0.8f, 0.8f, 0.8f);
    c_Earth_world = SelfRotation * Translation * Rotation * Scale;

    //Moon
    c_moon_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 15.0f);
    Scale = XMMatrixScaling(0.1f, 0.1f, 0.1f);
    c_moon_world = SelfRotation * Translation * Rotation * Scale * c_Earth_world;

    //MARS
    c_Mars_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 10.0f);
    Scale = XMMatrixScaling(1.1f, 1.1f, 1.1f);
    c_Mars_world = SelfRotation * Translation * Rotation * Scale;

    //UPITER
    c_Upiter_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 15.0f);
    Scale = XMMatrixScaling(1.2f, 1.2f, 1.2f);
    c_Upiter_world = SelfRotation * Translation * Rotation * Scale;

    //Saturn
    c_Saturn_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 20.0f);
    Scale = XMMatrixScaling(1.2f, 1.2f, 1.2f);
    c_Saturn_world = SelfRotation * Translation * Rotation * Scale;

    //Uran
    c_Uran_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 25.0f);
    Scale = XMMatrixScaling(1.2f, 1.2f, 1.2f);
    c_Uran_world = SelfRotation2 * Translation * Rotation * Scale;

    //Neptune
    c_Neptune_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 35.0f);
    Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    c_Neptune_world = SelfRotation * Translation * Rotation * Scale;

    //Pluto
    c_Pluto_world = XMMatrixIdentity();
    Rotation = XMMatrixRotationAxis(rotaxis, -rot);
    Translation = XMMatrixTranslation(0.0f, 0.0f, 80.0f);
    Scale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
    c_Pluto_world = SelfRotation * Translation * Rotation * Scale;

}

void DrawScene()
{
    //Clear our backbuffer
    float bgColor[4] = { 0.827451050f,  0.827451050f, 0.827451050f, 1.0f };
    d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
    //Refresh the Depth/Stencil view
    d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


    //Draw the sun
    WVP = c_sun_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the mercury
    WVP = c_mercury_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Venus
    WVP = c_venus_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Earth
    WVP = c_Earth_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Moon
    WVP = c_moon_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Mars
    WVP = c_Mars_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Upiter
    WVP = c_Upiter_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Saturn
    WVP = c_Saturn_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Uran
    WVP = c_Uran_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Draw the Neptune
    WVP = c_Neptune_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);
    
    //Draw the Pluto
    WVP = c_Pluto_world * camView * camProjection;
    cbPerObj.WVP = XMMatrixTranspose(WVP);
    d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
    d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
    d3d11DevCon->DrawIndexed(36, 0, 0);

    //Present the backbuffer to the screen
    SwapChain->Present(0, 0);
}

int messageloop() {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (true)
    {
        BOOL PeekMessageL(
            LPMSG lpMsg,
            HWND hWnd,
            UINT wMsgFilterMin,
            UINT wMsgFilterMax,
            UINT wRemoveMsg
        );

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // run game code            
            UpdateScene();
            DrawScene();
        }
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd,
        msg,
        wParam,
        lParam);
}